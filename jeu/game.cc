// Copyright 2014 Huns de Troyes
#include "include/game.h"

// Initialise l'état de la partie en début de jeu
void GameInit(PSContext2D_t* ctx, Jeu* state) {
	if(state->state != STATE_PAUSED) {
		// Initialisation de la fonction rand
		srand(time(NULL));

		// Affichage de l'écran de chargement et hargement des textures
		DrawLoadingScreen(ctx, state);
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

		// Le jeu démarrer avec 5 vies et un score de 0
		state->lives = 5;
		NewLife(ctx, state);
		state->score = 0;
	}
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
									// Mise a jour de la position des balles collées
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
		// Perte du verrouillage du curseur
		case PSE_MOUSELOCK_MOUSELOCKLOST: {
			state->newState = STATE_PAUSED;
			break;
		}
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

    // Mise a jour des balles
	for (i = 0; i < MAXBALL; i++) {
        // Si la balle existe
		if (state->ball[i].type != BALL_NONE) {
            // Deplacement de la balle
			state->ball[i].pos.x += state->ball[i].velocity.x;
            state->ball[i].pos.y += state->ball[i].velocity.y;

            // Collision avec les murs
			WallsCollision(ctx, state, i);

			// Collision avec la paddle
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

            // Supression des balles perdues
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

			// Collisions avec les briques
			if (w != 0 && h != 0) {
				/// Pour chaque brique
				for(j = 0; j < BRICKW; j++) {
					for(k = 0; k < BRICKH; k++) {
						// Position de la brique
						int x = j * (ctx->width / BRICKW),
							y = k * (ctx->height / BRICKH);
						PP_Rect surf = PP_MakeRectFromXYWH(x, y, w, h);
						// si la brique existe et qu'elle entre en collision avec la balle
						if (state->bricks[j][k] != BRICK_NONE &&
							collides(state->ball[i], surf)) {
							// Si la balle est explosive
							if(state->ball[i].type == BALL_EXPLODE) {
								// Endommag aussi les briques voisines
								int l, m;
								for(l = j - 1; l <= j + 1; l++)
									for(m = k - 1; m <= k + 1; m++)
										HitBrick(ctx, state, l, m);
							} else {
								// Endommage la brique
								HitBrick(ctx, state, j, k);
							}

							// Distances entre la balle et la brique sur les axes x et y
							int xd = (surf.point.x + surf.size.width / 2) -
								state->ball[i].pos.x;
							int yd = (surf.point.y + surf.size.height / 2) -
								state->ball[i].pos.y;

							// Determination de l'axe de rebond
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

	// Mise a jour des drops
	for (i = 0; i < MAXDROP; i++)
		// Si le drop existe
		if (state->drops[i].type != DROP_NONE) {
			// Descend de 2 pixels
			state->drops[i].pos.y += 2;
			// Collision avec la paddle
			if (state->drops[i].pos.y + 10 >=
				ctx->height - state->paddle.surf.size.height) {
				if (state->drops[i].pos.x >= state->paddle.surf.point.x &&
					state->drops[i].pos.x <=
					state->paddle.surf.point.x + state->paddle.surf.size.width) {
					// Augmente le score
					state->score++;
					// Application de l'effet du drop
					switch (state->drops[i].type) {
						case DROP_PADDLE_PLUS:
							state->paddle.surf.size.width =
								clamp(state->paddle.surf.size.width + 50, MINPADDLE, MAXPADDLE);
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
							if(state->bonus != 3)
								state->paddle.surf.size.width =
									clamp(state->paddle.surf.size.width - 50, MINPADDLE, MAXPADDLE);
							break;
						case DROP_SPEED_LESS:
							if(state->bonus != 4)
								for (j = 0; j < MAXBALL; j++) {
									if (state->ball[j].type != BALL_NONE) {
										state->ball[j].velocity.x /= 1.5;
										state->ball[j].velocity.y /= 1.5;
										state->ball[j].speed =
											clamp(state->ball[j].speed - 3, MINSPEED, MAXSPEED);
									}
								}
							break;
						case DROP_SPEED_PLUS:
							if(state->bonus != 4)
								for (j = 0; j < MAXBALL; j++) {
									if (state->ball[j].type != BALL_NONE) {
										state->ball[j].velocity.x *= 1.5;
										state->ball[j].velocity.y *= 1.5;
										state->ball[j].speed =
											clamp(state->ball[j].speed + 3, MINSPEED, MAXSPEED);
									}
								}
							break;
						default:
							break;
					}
					// Suppression du drop
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

// Dessine l'écran de pause (le jeu assombri)
void PauseDraw(PSContext2D_t* ctx, Jeu* state) {
	GameDraw(ctx, state);
	DrawRect(ctx, PP_MakeRectFromXYWH(0, 0, ctx->width, ctx->height),
			 RGBA(0, 0, 0, 128));
}

// Fonction appelée a chaque frame de jeu pour mettre a jour l'image
void GameDraw(PSContext2D_t* ctx, Jeu* state) {
	// Effacement de l'écran
	// memset(ctx->data, 0x00, ctx->width * ctx->height * 4);
	DrawTexture(ctx, PP_MakePoint(0, 0), state->textures[8]);

	int i, j;

	for(i = 0; i < state->lives; i++) {
		DrawTexture(ctx,
					PP_MakePoint(state->textures[0].width * i, 0),
					state->textures[0]);
	}

	// Affichage des briques
	for(i = 0; i < BRICKW; i++)
		for(j = 0; j < BRICKH; j++) {
			// Calcul de la position de la texture
			PP_Point p = PP_MakePoint(i * (ctx->width / BRICKW),
									  j * (ctx->height / BRICKH));
			// Choix de la texture
			switch(state->bricks[i][j]) {
				case BRICK_ONETOUCH:
					DrawTexture(ctx, p, state->textures[1]);
					break;
				case BRICK_TWOTOUCH:
					DrawTexture(ctx, p, state->textures[2]);
					break;
				case BRICK_THREETOUCH:
					DrawTexture(ctx, p, state->textures[3]);
					break;
				case BRICK_UBER:
					DrawTexture(ctx, p, state->textures[4]);
					break;
				default:
					break;
			}
		}

	// Rendu des drops
	for(i = 0; i < MAXDROP; i++) {
		switch(state->drops[i].type) {
            case DROP_PADDLE_PLUS:
                DrawTexture(ctx,
							PP_MakePoint(state->drops[i].pos.x - 15,
										 state->drops[i].pos.y - 15),
							state->textures[9]);
                break;
            case DROP_STICK:
                DrawTexture(ctx,
							PP_MakePoint(state->drops[i].pos.x - 15,
										 state->drops[i].pos.y - 15),
							state->textures[10]);
                break;
            case DROP_CLONE:
                DrawTexture(ctx,
							PP_MakePoint(state->drops[i].pos.x - 15,
										 state->drops[i].pos.y - 15),
							state->textures[11]);
                break;
            case DROP_EXPLODE:
                DrawTexture(ctx,
							PP_MakePoint(state->drops[i].pos.x - 15,
										 state->drops[i].pos.y - 15),
							state->textures[12]);
                break;
            case DROP_SPEED_LESS:
                DrawTexture(ctx,
							PP_MakePoint(state->drops[i].pos.x - 15,
										 state->drops[i].pos.y - 15),
							state->textures[13]);
                break;
            case DROP_PADDLE_LESS:
                DrawTexture(ctx,
							PP_MakePoint(state->drops[i].pos.x - 15,
										 state->drops[i].pos.y - 15),
							state->textures[14]);
                break;
            case DROP_LOSE:
                DrawTexture(ctx,
							PP_MakePoint(state->drops[i].pos.x - 15,
										 state->drops[i].pos.y - 15),
							state->textures[15]);
                break;
            case DROP_SPEED_PLUS:
                DrawTexture(ctx,
							PP_MakePoint(state->drops[i].pos.x - 15,
										 state->drops[i].pos.y - 15),
							state->textures[16]);
                break;
            default:
                break;
		}
	}

	// Rendu de la paddle
	state->paddle.surf.point.y = ctx->height - state->paddle.surf.size.height;
	DrawTexture(ctx, state->paddle.surf.point, state->textures[5]);
	for(i = 0; i < state->paddle.surf.size.width - 30; i++)
		DrawTexture(ctx,
					PP_MakePoint(state->paddle.surf.point.x + 15 + i,
								 state->paddle.surf.point.y),
					state->textures[6]);
	DrawTexture(ctx,
				PP_MakePoint(state->paddle.surf.point.x +
							 (state->paddle.surf.size.width - 15),
							 state->paddle.surf.point.y), state->textures[7]);

	if(state->paddle.sticky == PP_TRUE) {
		DrawTexture(ctx, state->paddle.surf.point, state->textures[17]);
		for(i = 0; i < state->paddle.surf.size.width - 30; i += 50)
			DrawTexture(ctx,
						PP_MakePoint(state->paddle.surf.point.x + 15 + i,
									 state->paddle.surf.point.y),
						state->textures[18]);
		DrawTexture(ctx,
				PP_MakePoint(state->paddle.surf.point.x +
							 (state->paddle.surf.size.width - 15),
							 state->paddle.surf.point.y), state->textures[19]);
	}

	// Affichage du score
	char score[20];
	snprintf(score, sizeof score, "%" PRIu64, state->score);
	DrawText(ctx,
			 PP_MakePoint(ctx->width - (26 * strlen(score)), 0),
			 score, state->textures[21]);

	// Rendu de la balle
	for(i = 0; i < MAXBALL; i++)
		if (state->ball[i].type == BALL_CLASSIC) {
			DrawTexture(ctx,
						PP_MakePoint(state->ball[i].pos.x - state->ball[i].radius,
									 state->ball[i].pos.y - state->ball[i].radius),
						state->textures[0]);
		} else if (state->ball[i].type == BALL_EXPLODE) {
			DrawTexture(ctx,
						PP_MakePoint(state->ball[i].pos.x - state->ball[i].radius,
									 state->ball[i].pos.y - state->ball[i].radius - 10),
						state->textures[20]);
		}
}
