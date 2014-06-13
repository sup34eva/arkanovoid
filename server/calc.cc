// Copyright 2014 Huns de Troyes
#include "include/calc.h"

// Appelée quand la souris est vérrouillée
void MouseLocked(void* user_data, int32_t result) {
	PostNumber(42);
}

// Appelée quand la souris est dévérouillée
void MouseLockLost(PP_Instance instance) {
	PostNumber(101010);
	SetState(STATE_PAUSED);
}

// Initialisation de l'écran titre
void TitleInit(Jeu* state) {
	/* Définitions des événements que le programme doit recevoir :
	 * (souris, clavier et écran tactile) */
	PSInstance::GetInstance()->SetEnabledEvents(PSE_ALL);
	PSInstance::GetInstance()->RequestInputEvents(PP_INPUTEVENT_CLASS_MOUSE |
												  PP_INPUTEVENT_CLASS_KEYBOARD |
												  PP_INPUTEVENT_CLASS_TOUCH);

	// Chargement des texture de l'écran titre
	state->textures[0] = LoadTexture("/img/logo.tex");
	state->textures[1] = LoadTexture("/img/button.tex");
}

// Initialise l'état de la partie en début de jeu
void GameInit(PSContext2D_t* ctx, Jeu* state) {
	// Initialisation de la fonction rand
	srand(time(NULL));

	// Chargement des textures
	LoadTextures(state);

	// Initialisation des briques
	int i, j;
	state->brickCount = 0;
	for(i = 0; i < BRICKW; i++) {
		for(j = 2; j < BRICKH; j++) {
			if(j < BRICKH - 4 && j > 1) {
				int rnd = rand() % 100;
				if (rnd < 50) {
					state->bricks[i][j] = BRICK_NONE;
				} else if (rnd < 75) {
					state->bricks[i][j] = BRICK_ONETOUCH;
					state->brickCount++;
				} else if (rnd < 90) {
					state->bricks[i][j] = BRICK_THREETOUCH;
					state->brickCount++;
				} else {
					state->bricks[i][j] = BRICK_UBER;
				}
			} else {
				state->bricks[i][j] = BRICK_NONE;
			}
		}
	}

	// Mise a zero des drop (type = none)
	for(i = 0; i < MAXDROP; i++) {
		state->drops[i].pos = PP_MakeFloatPoint(0, 0);
		state->drops[i].type = DROP_NONE;
	}

	// Position initiale de la balle
	for(i = 0; i < MAXBALL; i++) {
        InitBall(state, i);
        if (i == 0)
            state->ball[i].type = BALL_CLASSIC;
        else
            state->ball[i].type = BALL_NONE;
	}
	state->ballCount = 1;

	// Position initiale de la paddle
	state->paddle = PP_MakeRectFromXYWH(1100/2 - 50, 0, 100,
										state->textures[6].height);
}

// Cette fonction bloque un nombre entre 2 limites
float clamp(float val, float low, float high) {
	return fmin(fmax(val, low), high);
}

// Gère les évenements en jeu
void GameHandleEvent(PSEvent* event, Jeu* state, PSContext2D_t* ctx) {
	// Interface de gestion des evenements claviers / souris
	PPB_InputEvent* pInputEvent = (PPB_InputEvent*)
		PSGetInterface(PPB_INPUT_EVENT_INTERFACE);
	PPB_KeyboardInputEvent* pKeyboardInput = (PPB_KeyboardInputEvent*)
		PSGetInterface(PPB_KEYBOARD_INPUT_EVENT_INTERFACE);
	PPB_MouseInputEvent* pMouseInput = (PPB_MouseInputEvent*)
		PSGetInterface(PPB_MOUSE_INPUT_EVENT_INTERFACE);

	// Si l'evenement est une entrée utilisateur
	if (event->type == PSE_INSTANCE_HANDLEINPUT) {
		// Action a effectuer en fonction du type d'event
		switch (pInputEvent->GetType(event->as_resource)) {
			case PP_INPUTEVENT_TYPE_KEYDOWN: {  // Appui d'une touche du clavier
				// Recupération du code de la touche
				uint32_t key_code = pKeyboardInput->GetKeyCode(event->as_resource);
				switch(key_code) {
						case 39 :  // Fleche Droite
							state->paddle.point.x += ctx->width / 50;
							break;
						case 37 :  // Fleche Gauche
							state->paddle.point.x -= ctx->width / 50;
							break;
						case 27 :  // Echap
							SetState(STATE_PAUSED);
							break;
				}
				break;
			}
			case PP_INPUTEVENT_TYPE_MOUSEMOVE: {  // Mouvement de la souris
				struct PP_Point movement = pMouseInput->GetMovement(event->as_resource);
				state->paddle.point.x += movement.x;
				break;
			}
			default:
				break;
		}
	}
}

// Gestion des evenements sur l'écran titre
void TitleHandleEvent(PSEvent* event, Jeu* state, PSContext2D_t* ctx) {
	// Interface de gestion des evenements
	PPB_InputEvent* pInputEvent = (PPB_InputEvent*)
		PSGetInterface(PPB_INPUT_EVENT_INTERFACE);

	if (event->type == PSE_INSTANCE_HANDLEINPUT) {
		switch (pInputEvent->GetType(event->as_resource)) {
			case PP_INPUTEVENT_TYPE_MOUSEDOWN: {  // Click de souris
				// Interface de verrouillage du curseur
				PPB_MouseLock* pMouseLock = (PPB_MouseLock*)
					PSGetInterface(PPB_MOUSELOCK_INTERFACE);
				// Verrouille le curseur pour améliorer l'immersion / ergonomie
				pMouseLock->LockMouse(PSGetInstanceId(),
									  PP_MakeCompletionCallback(MouseLocked,
																0));
				// Passe a l'état "en jeu"
				SetState(STATE_INGAME);
				break;
			}
			default:
				break;
		}
	}
}

// Determine si un point est contenu dans un rectangle
int Contains(struct PP_Rect rect, PP_FloatPoint point) {
  return (point.x >= rect.point.x) &&
	  (point.x < (rect.point.x + rect.size.width)) &&
	  (point.y >= rect.point.y) &&
	  (point.x < (rect.point.y + rect.size.height));
}

// Met a jour l'état du jeu
void GameCalc(PSContext2D_t* ctx, Jeu* state) {
	int i, j, h = ctx->height / BRICKH, w = ctx->width / BRICKW;

	// Vérifie que la paddle ne sort pas de l'écran
	state->paddle.point.x = clamp(state->paddle.point.x, 0,
								  ctx->width - state->paddle.size.width);

    for (i = 0; i < MAXBALL; i++) {
        if (state->ball[i].type != BALL_NONE) {
            int lastX = clamp(state->ball[i].pos.x, 1, ctx->width),
                lastY = clamp(state->ball[i].pos.y, 1, ctx->height);

            state->ball[i].pos.x += state->ball[i].velocity.x;
            state->ball[i].pos.y += state->ball[i].velocity.y;

            if (state->ball[i].pos.x - state->ball[i].radius <= 0) {
                state->ball[i].pos.x = state->ball[i].radius + 1;
                state->ball[i].velocity.x = fabs(state->ball[i].velocity.x);
            }

            if (state->ball[i].pos.x + state->ball[i].radius >= ctx->width) {
                state->ball[i].pos.x = ctx->width - state->ball[i].radius - 1;
                state->ball[i].velocity.x = -fabs(state->ball[i].velocity.x);
            }

            if (state->ball[i].pos.y - state->ball[i].radius <= 0) {
                state->ball[i].pos.y = state->ball[i].radius + 1;
                state->ball[i].velocity.y = fabs(state->ball[i].velocity.y);
            }

            if (state->ball[i].pos.y + state->ball[i].radius>
               ctx->height - state->paddle.size.height &&
               state->ball[i].pos.x >= state->paddle.point.x &&
               state->ball[i].pos.x <= state->paddle.point.x +
               state->paddle.size.width) {
                state->ball[i].pos.y = ctx->height - (
                    state->paddle.size.height + state->ball[i].radius + 1);
                float angle = ((90 * (
                    (state->ball[i].pos.x - state->paddle.point.x) /
                    state->paddle.size.width)) / 2) - (45 / 2),
                    x = sin((PI / 180) * angle) * state->ball[i].speed,
                    y = cos((PI / 180) * angle) * state->ball[i].speed;
                state->ball[i].velocity.x = x;
                state->ball[i].velocity.y = -y;
            }

            if (state->ball[i].pos.y > ctx->height) {
                state->ball[i].type = BALL_NONE;
                state->ballCount--;
                /*if(state->ballCount <= 0)
                    SetState(STATE_SCORE);*/
            }

            // TODO: Prendre en compte le rayon de la balle
            if (h != 0 && w != 0) {
                int brickX = state->ball[i].pos.x / w,
                    brickY = state->ball[i].pos.y / h,
                    lastBrickX = lastX / w, lastBrickY = lastY / h;

                if (state->bricks[brickX][brickY] != BRICK_NONE) {
                    switch (state->bricks[brickX][brickY]) {
                        case BRICK_ONETOUCH:
                            state->bricks[brickX][brickY] = BRICK_NONE;
                            state->brickCount--;
                            if (1)
                                SpawnDrop((brickX * (ctx->width / BRICKW) +
                                           (0.5 * (ctx->width / BRICKW))) ,
                                          brickY * (ctx->height / BRICKH),
                                          state);
                            break;
                        case BRICK_TWOTOUCH:
                            state->bricks[brickX][brickY] = BRICK_ONETOUCH;
                            break;
                        case BRICK_THREETOUCH:
                            state->bricks[brickX][brickY] = BRICK_TWOTOUCH;
                            break;
                        default:
                            break;
                    }

                    if (brickX > lastBrickX)
                        state->ball[i].velocity.x =
                        -fabs(state->ball[i].velocity.x);

                    if (brickX < lastBrickX)
                        state->ball[i].velocity.x =
                        fabs(state->ball[i].velocity.x);

                    if (brickY > lastBrickY)
                        state->ball[i].velocity.y =
                        -fabs(state->ball[i].velocity.y);

                    if (brickY < lastBrickY)
                        state->ball[i].velocity.y =
                        fabs(state->ball[i].velocity.y);
                }
            }
        }
    }

    for (i = 0; i < MAXDROP; i++)
        if (state->drops[i].type != DROP_NONE) {
            state->drops[i].pos.y++;
            if (state->drops[i].pos.y + 10 >=
                ctx->height - state->paddle.size.height) {
                if (state->drops[i].pos.x >= state->paddle.point.x &&
                    state->drops[i].pos.x <=
                    state->paddle.point.x + state->paddle.size.width) {
                    switch (state->drops[i].type) {
                        case DROP_PADDLE_PLUS:
                            state->paddle.size.width =
                                clamp(state->paddle.size.width * 1.5, 30, 300);
                            break;
                        case DROP_STICK:
                            for (j = 0; j < MAXBALL; j++)
                                if (state->ball[j].type != BALL_NONE)
                                    state->ball[j].type = BALL_STICKY;
                            break;
                        case DROP_CLONE:
                            AddBall(state);
                            break;
                        case DROP_EXPLODE:
                            for (j = 0; j < MAXBALL; j++)
                                if (state->ball[j].type != BALL_NONE)
                                    state->ball[j].type = BALL_EXPLODE;
                            break;
                        case DROP_LOSE:
                            state->lives--;
                            break;
                        case DROP_PADDLE_LESS:
                            state->paddle.size.width =
                            clamp(state->paddle.size.width / 1.5, 30, 300);
                            break;
                        case DROP_SPEED_LESS:
                            for (j = 0; j < MAXBALL; j++) {
                                if (state->ball[j].type != BALL_NONE) {
                                    state->ball[j].velocity.x /= 1.5;
                                    state->ball[j].velocity.y /= 1.5;
                                    clamp(state->ball[j].speed / 1.5, 2, 18);
                                }
                            }
                            break;
                        case DROP_SPEED_PLUS:
                            for (j = 0; j < MAXBALL; j++) {
                                if (state->ball[j].type != BALL_NONE) {
                                    state->ball[j].velocity.x *= 1.5;
                                    state->ball[j].velocity.y *= 1.5;
                                    clamp(state->ball[j].speed * 1.5, 2, 18);
                                }
                            }
                            break;
                        default:
                            break;
                    }
                    state->drops[i].type = DROP_NONE;
                } else if (state->drops[i].pos.y >= ctx->height) {
                    state->drops[i].type = DROP_NONE;
                }
            }
        }

	// Si il n'y a plus de briques
	if(state->brickCount <= 0)
		SetState(STATE_SCORE);
}

// Determine la distance entre 2 points
float Dist(struct PP_FloatPoint from, struct PP_FloatPoint to) {
	return sqrt(pow(from.x - to.x, 2) + pow(from.y - to.y, 2));
}

void SpawnDrop(int x, int y, Jeu* state) {
    int i;
    for (i = 0; i < MAXDROP; i++)
    if (state->drops[i].type == DROP_NONE) {
        state->drops[i].pos = PP_MakeFloatPoint(x, y);
        switch (rand() % 8) {
            case 0 :
                state->drops[i].type = DROP_PADDLE_PLUS;
                break;
            case 1 :
                state->drops[i].type = DROP_STICK;
                break;
            case 2 :
                state->drops[i].type = DROP_CLONE;
                break;
            case 3 :
                state->drops[i].type = DROP_EXPLODE;
                break;
            case 4 :
                state->drops[i].type = DROP_SPEED_LESS;
                break;
            case 5 :
                state->drops[i].type = DROP_PADDLE_LESS;
                break;
            case 6 :
                state->drops[i].type = DROP_LOSE;
                break;
            case 7 :
                state->drops[i].type = DROP_SPEED_PLUS;
                break;
        }
        return;
    }
}

void InitBall(Jeu* state, int i) {
    state->ball[i].speed = BALLSPEED;
    state->ball[i].pos = PP_MakeFloatPoint(1100/2, 700 - 20);
    float x = sin((PI / 180) * STARTANGLE) * BALLSPEED,
        y = cos((PI / 180) * STARTANGLE) * BALLSPEED;
    state->ball[i].velocity = PP_MakeFloatPoint(x, y);
    state->ball[i].radius = 5;
}

int AddBall(Jeu* state) {
    int i;
    for (i = 0; i < MAXBALL; i++) {
        if (state->ball[i].type == BALL_NONE) {
            InitBall(state, i);
            state->ballCount++;
            state->ball[i].type = BALL_CLASSIC;
            return i;
        }
    }
    return 0;
}
