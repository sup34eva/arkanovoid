// Copyright 2014 Huns de Troyes
#include "include/calc.h"

// Initialisation de l'écran titre
void TitleInit(Jeu* state) {
	/* Définitions des événements que le programme doit recevoir :
	 * (souris, clavier et écran tactile) */
	PSInstance::GetInstance()->SetEnabledEvents(PSE_ALL);
	PSInstance::GetInstance()->RequestInputEvents(PP_INPUTEVENT_CLASS_MOUSE |
												  PP_INPUTEVENT_CLASS_KEYBOARD |
												  PP_INPUTEVENT_CLASS_TOUCH);

	// Chargement des textures
	LoadTitleTextures(state);

	int i;
	for(i = 0; i < 9; i++)
		state->shop[i] = PP_FALSE;

	// Interface de verrouillage du curseur
	PPB_MouseLock* pMouseLock = (PPB_MouseLock*)
		PSGetInterface(PPB_MOUSELOCK_INTERFACE);
	// Déverrouille le curseur
	pMouseLock->UnlockMouse(PSGetInstanceId());
}

void NewLife(PSContext2D_t* ctx, Jeu* state) {
	// Mise a zero des drop (type = none)
	int i;
	for(i = 0; i < MAXDROP; i++) {
		state->drops[i].pos = PP_MakeFloatPoint(0, 0);
		state->drops[i].type = DROP_NONE;
	}

	// Position initiale de la balle
	for(i = 0; i < MAXBALL; i++) {
        InitBall(state, i);
        if (i == 0) {
            state->ball[i].type = BALL_CLASSIC;
			state->ball[i].stuck = PP_TRUE;
			state->ball[i].velocity = PP_MakeFloatPoint(0, 0);
		} else {
            state->ball[i].type = BALL_NONE;
		}
	}
	state->ballCount = 1;

	// Position initiale de la paddle
	state->paddle.surf = PP_MakeRectFromXYWH(1100/2 - 50, 0, 130,
											 state->textures[6].height);
	state->paddle.sticky = PP_FALSE;
}

// Initialise l'état de la partie en début de jeu
void GameInit(PSContext2D_t* ctx, Jeu* state) {
	if(state->state != STATE_PAUSED) {
		// Initialisation de la fonction rand
		srand(time(NULL));

		// Chargement des textures
		LoadGameTextures(state);

		// Initialisation des briques
		int i, j;
		state->brickCount = 0;
		for(i = 0; i < BRICKW; i++) {
			for(j = 0; j < BRICKH; j++) {
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

		NewLife(ctx, state);
		state->lives = 5;
		state->score = 0;
	}
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
	switch(event->type) {
		case PSE_INSTANCE_HANDLEINPUT: {
			// Action a effectuer en fonction du type d'event
			switch (pInputEvent->GetType(event->as_resource)) {
				case PP_INPUTEVENT_TYPE_KEYDOWN: {  // Appui d'une touche du clavier
					// Recupération du code de la touche
					uint32_t key_code = pKeyboardInput->GetKeyCode(event->as_resource);
					switch(key_code) {
							case 39 : {  // Fleche Droite
								state->paddle.surf.point.x += ctx->width / 50;
								int i;
								for (i = 0; i < MAXBALL; i++) {
									if(state->ball[i].stuck == PP_TRUE)
										state->ball[i].pos.x += ctx->width / 50;
								}
								break;
							}
							case 37 : {  // Fleche Gauche
								state->paddle.surf.point.x -= ctx->width / 50;
								int i;
								for (i = 0; i < MAXBALL; i++) {
									if(state->ball[i].stuck == PP_TRUE)
										state->ball[i].pos.x -= ctx->width / 50;
								}
								break;
							}
							case 38 : {  // Fleche haut
								int i;
								for (i = 0; i < MAXBALL; i++) {
									if(state->ball[i].stuck == PP_TRUE) {
										PaddleCollision(ctx, state, i);
										state->ball[i].stuck = PP_FALSE;
									}
								}
								break;
							}
							case 27 :  // Echap
								state->newState = STATE_PAUSED;
								break;
					}
					break;
				}
				case PP_INPUTEVENT_TYPE_MOUSEMOVE: {  // Mouvement de la souris
					struct PP_Point movement = pMouseInput->GetMovement(event->as_resource);
					state->paddle.surf.point.x += movement.x;
					int i;
					for (i = 0; i < MAXBALL; i++) {
						if(state->ball[i].stuck == PP_TRUE)
							state->ball[i].pos.x += movement.x;
					}
					break;
				}
				case PP_INPUTEVENT_TYPE_MOUSEDOWN: {  // Click de souris
					int i;
					for (i = 0; i < MAXBALL; i++) {
						if(state->ball[i].stuck == PP_TRUE) {
							PaddleCollision(ctx, state, i);
							state->ball[i].stuck = PP_FALSE;
						}
					}
					break;
				}
				default:
					break;
			}
			break;
		}
		case PSE_MOUSELOCK_MOUSELOCKLOST: {
			state->newState = STATE_PAUSED;
			break;
		}
		default:
			break;
	}
}

// Appelée quand la souris est vérrouillée
void MouseLocked(void* data, int32_t result) {
	if(result == PP_OK) {
		Jeu* state = (Jeu*) data;
		state->newState = STATE_INGAME;
	} else {
		PostMessage("PointerLock error %d", result);
	}
}

// Gestion des evenements sur l'écran titre
void TitleHandleEvent(PSEvent* event, Jeu* state, PSContext2D_t* ctx) {
	// Interface de gestion des evenements
	PPB_InputEvent* pInputEvent = (PPB_InputEvent*)
		PSGetInterface(PPB_INPUT_EVENT_INTERFACE);

	switch(event->type) {
		case PSE_INSTANCE_HANDLEINPUT: {
			switch (pInputEvent->GetType(event->as_resource)) {
				case PP_INPUTEVENT_TYPE_MOUSEUP: {  // Click de souris
					// Interface de verrouillage du curseur
					PPB_MouseLock* pMouseLock = (PPB_MouseLock*)
						PSGetInterface(PPB_MOUSELOCK_INTERFACE);
					// Verrouille le curseur pour améliorer l'immersion / ergonomie
					pMouseLock->LockMouse(PSGetInstanceId(),
										 PP_MakeCompletionCallback(MouseLocked, state));
					// Passe a l'état "en jeu"
					state->newState = STATE_INGAME;
					break;
				}
				default:
					break;
			}
			break;
		}
		case PSE_INSTANCE_HANDLEMESSAGE: {
			PP_Var message = event->as_var;
			if(message.type == PP_VARTYPE_ARRAY) {
				// Interface de lecture des tableaux
				PPB_VarArray* pVarArray = (PPB_VarArray*)
					PSGetInterface(PPB_VAR_ARRAY_INTERFACE);
				// Longeur du message
				uint32_t i, len = pVarArray->GetLength(message);
				for(i = 0; i < len; i++) {
					PP_Var item = pVarArray->Get(message, i);
					if(item.type == PP_VARTYPE_BOOL)
						state->shop[i] = item.value.as_bool;
				}
			} else if (message.type == PP_VARTYPE_INT32) {
			}
			break;
		}
		default:
			break;
	}
}

int collides(Ball a, struct PP_Rect b) {
	return a.pos.x - a.radius < b.point.x + b.size.width &&
		a.pos.x + a.radius > b.point.x &&
		a.pos.y - a.radius < b.point.y + b.size.height &&
		a.pos.y + a.radius > b.point.y;
}

void WallsCollision(PSContext2D_t* ctx, Jeu* state, int i) {
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
}

void PaddleCollision(PSContext2D_t* ctx, Jeu* state, int i) {
	float angle = ((90 * (
		(state->ball[i].pos.x - state->paddle.surf.point.x) /
		state->paddle.surf.size.width)) / 2) - (45 / 2),
		x = sin(angle / 180 * PI) * state->ball[i].speed,
		y = cos(angle / 180 * PI) * state->ball[i].speed;
	state->ball[i].velocity.x = x;
	state->ball[i].velocity.y = -y;
}

void HitBrick(PSContext2D_t* ctx, Jeu* state, int j, int k) {
	int h = ctx->height / BRICKH,
		w = ctx->width / BRICKW,
		x = j * (ctx->width / BRICKW),
		y = k * (ctx->height / BRICKH);
	switch (state->bricks[j][k]) {
		case BRICK_ONETOUCH:
			state->score++;
			state->bricks[j][k] = BRICK_NONE;
			state->brickCount--;
			if (rand() < (RAND_MAX / 2))
				SpawnDrop(x + (w/2), y + (h/2), state);
			break;
		case BRICK_TWOTOUCH:
			state->bricks[j][k] = BRICK_ONETOUCH;
			break;
		case BRICK_THREETOUCH:
			state->bricks[j][k] = BRICK_TWOTOUCH;
			break;
		default:
			break;
	}
}

// Met a jour l'état du jeu
void GameCalc(PSContext2D_t* ctx, Jeu* state) {
	int i, j, k, h = ctx->height / BRICKH, w = ctx->width / BRICKW;

	// Vérifie que la paddle ne sort pas de l'écran
	state->paddle.surf.point.x = clamp(state->paddle.surf.point.x, 0,
									   ctx->width - state->paddle.surf.size.width);

    for (i = 0; i < MAXBALL; i++) {
        if (state->ball[i].type != BALL_NONE) {
            state->ball[i].pos.x += state->ball[i].velocity.x;
            state->ball[i].pos.y += state->ball[i].velocity.y;

            WallsCollision(ctx, state, i);

			if (state->ball[i].pos.y + state->ball[i].radius >
			ctx->height - state->paddle.surf.size.height &&
			state->ball[i].pos.x >= state->paddle.surf.point.x &&
			state->ball[i].pos.x <= state->paddle.surf.point.x +
			state->paddle.surf.size.width) {
				state->ball[i].pos.y = ctx->height -
					(state->paddle.surf.size.height + state->ball[i].radius + 1);
				if(state->paddle.sticky == PP_FALSE && state->ball[i].stuck == PP_FALSE) {
					PaddleCollision(ctx, state, i);
				} else {
					state->ball[i].velocity = PP_MakeFloatPoint(0, 0);
					state->ball[i].stuck = PP_TRUE;
				}
			}

            if (state->ball[i].pos.y > ctx->height) {
                state->ball[i].type = BALL_NONE;
                state->ballCount--;
                if (state->ballCount <= 0) {
					state->lives--;
					if(state->lives > 0)
						NewLife(ctx, state);
					else
                    	state->newState = STATE_SCORE;
				}
            }

			if (w != 0 && h != 0) {
				for(j = 0; j < BRICKW; j++) {
					for(k = 0; k < BRICKH; k++) {
						int x = j * (ctx->width / BRICKW),
							y = k * (ctx->height / BRICKH);
						PP_Rect surf = PP_MakeRectFromXYWH(x, y, w, h);
						if (state->bricks[j][k] != BRICK_NONE &&
							collides(state->ball[i], surf)) {
							if(state->ball[i].type == BALL_EXPLODE) {
								int l, m;
								for(l = j - 1; l <= j + 1; l++)
									for(m = k - 1; m <= k + 1; m++)
										HitBrick(ctx, state, l, m);
							} else {
								HitBrick(ctx, state, j, k);
							}

							int xd = (surf.point.x + surf.size.width / 2) -
								state->ball[i].pos.x;
							int yd = (surf.point.y + surf.size.height / 2) -
								state->ball[i].pos.y;

							if (fabs(xd) > fabs(yd)) {
								if (xd < 0) {
									state->ball[i].velocity.x = fabs(state->ball[i].velocity.x);
								} else {
									state->ball[i].velocity.x = -fabs(state->ball[i].velocity.x);
								}
							} else {
								if (yd < 0) {
									state->ball[i].velocity.y = fabs(state->ball[i].velocity.y);
								} else {
									state->ball[i].velocity.y = -fabs(state->ball[i].velocity.y);
								}
							}
						}
					}
				}
			}
		}
	}

	for (i = 0; i < MAXDROP; i++)
		if (state->drops[i].type != DROP_NONE) {
			state->drops[i].pos.y++;
			if (state->drops[i].pos.y + 10 >=
				ctx->height - state->paddle.surf.size.height) {
				if (state->drops[i].pos.x >= state->paddle.surf.point.x &&
					state->drops[i].pos.x <=
					state->paddle.surf.point.x + state->paddle.surf.size.width) {
					state->score++;
					switch (state->drops[i].type) {
						case DROP_PADDLE_PLUS:
							state->paddle.surf.size.width =
								clamp(state->paddle.surf.size.width + 50, 30, 330);
							break;
						case DROP_STICK:
							state->paddle.sticky = PP_TRUE;
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
							if(state->lives <= 0)
								state->newState = STATE_SCORE;
							else
								NewLife(ctx, state);
							break;
						case DROP_PADDLE_LESS:
							state->paddle.surf.size.width =
							clamp(state->paddle.surf.size.width - 50, 30, 330);
							break;
						case DROP_SPEED_LESS:
							for (j = 0; j < MAXBALL; j++) {
								if (state->ball[j].type != BALL_NONE) {
									state->ball[j].velocity.x /= 1.5;
									state->ball[j].velocity.y /= 1.5;
									state->ball[j].speed = clamp(state->ball[j].speed - 3, 2, 18);
								}
							}
							break;
						case DROP_SPEED_PLUS:
							for (j = 0; j < MAXBALL; j++) {
								if (state->ball[j].type != BALL_NONE) {
									state->ball[j].velocity.x *= 1.5;
									state->ball[j].velocity.y *= 1.5;
									state->ball[j].speed = clamp(state->ball[j].speed + 3, 2, 18);
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
		state->newState = STATE_SCORE;
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
	state->ball[i].velocity = PP_MakeFloatPoint(0, 0);
    state->ball[i].pos = PP_MakeFloatPoint(1100/2, 700 - 20);
    state->ball[i].radius = BALLRADIUS;
	state->ball[i].stuck = PP_FALSE;
}

int AddBall(Jeu* state) {
    int i;
    for (i = 0; i < MAXBALL; i++) {
        if (state->ball[i].type == BALL_NONE) {
            InitBall(state, i);
			state->ball[i].velocity = PP_MakeFloatPoint(0, -state->ball[i].speed);
            state->ballCount++;
            state->ball[i].type = BALL_CLASSIC;
            return i;
        }
    }
    return 0;
}
