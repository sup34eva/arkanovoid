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
		for(j = 0; j < BRICKH; j++) {
			if(j < BRICKH - 2) {
				switch(rand() % 5) {
						case 0 :
							state->bricks[i][j] = BRICK_NONE;
							break;
						case 1 :
							state->bricks[i][j] = BRICK_ONETOUCH;
							state->brickCount++;
							break;
						case 2 :
						case 3 :
							state->bricks[i][j] = BRICK_THREETOUCH;
							state->brickCount++;
							break;
						case 4 :
							state->bricks[i][j] = BRICK_UBER;
							state->brickCount++;
							break;
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
	state->ball.pos = PP_MakeFloatPoint(1100/2, 700 - 20);
	float x = sin((PI / 180) * STARTANGLE) * BALLSPEED,
		y = cos((PI / 180) * STARTANGLE) * BALLSPEED;
	state->ball.velocity = PP_MakeFloatPoint(x, y);
	state->ball.radius = 5;

	// Position initiale de la paddle
	state->paddle = PP_MakeRectFromXYWH(0, 0, 100, state->textures[6].height);
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
	// Taille d'une brique
	int h = ctx->height / BRICKH, w = ctx->width / BRICKW,
		// Sauvegarde de la position précédente de la balle
		lastX = clamp(state->ball.pos.x, 1, ctx->width),
		lastY = clamp(state->ball.pos.y, 1, ctx->height);

	// Vérifie que la paddle ne sort pas de l'écran
	state->paddle.point.x = clamp(state->paddle.point.x, 0,
								  ctx->width - state->paddle.size.width);

	// Mise a jours de la position de la balle par rapport a sa vélocité
	state->ball.pos.x += state->ball.velocity.x;
	state->ball.pos.y += state->ball.velocity.y;

	// Collisions de la balle avec les bords de l'écran
	if(state->ball.pos.x - state->ball.radius <= 0) {
		// Replacement de la balle pour s'assurer qu'elle ne sorte pas de l'écran
		state->ball.pos.x = state->ball.radius + 1;
		// Mise a jour de sa vélocité
		state->ball.velocity.x = fabs(state->ball.velocity.x);
	}

	if(state->ball.pos.x + state->ball.radius >= ctx->width) {
		state->ball.pos.x = ctx->width - state->ball.radius - 1;
		state->ball.velocity.x = -fabs(state->ball.velocity.x);
	}

	if(state->ball.pos.y - state->ball.radius <= 0) {
		state->ball.pos.y = state->ball.radius + 1;
		state->ball.velocity.y = fabs(state->ball.velocity.y);
	}

	// En cas de collsion avec le bord inférieur
	if(state->ball.pos.y + state->ball.radius >
	   ctx->height - state->paddle.size.height) {
		// Si la balle entre en collision avec la paddle
		if(state->ball.pos.x >= state->paddle.point.x &&
		   state->ball.pos.x <= state->paddle.point.x +
		   state->paddle.size.width) {
			// Replacement de la balle (facilite le "repechage" de la balle)
			state->ball.pos.y = ctx->height -
				(state->paddle.size.height + state->ball.radius + 1);
			/* Calcul du nouvel angle de la balle
			 * (de 45° au niveau du bord gauche a 135° au niveau du bord droit) */
			float angle = ((90 * ((state->ball.pos.x - state->paddle.point.x)
								  / state->paddle.size.width)) / 2) - (45 / 2),
				// Determination de la vélocité correspondant a cet angle
				x = sin((PI / 180) * angle) * BALLSPEED,
				y = cos((PI / 180) * angle) * BALLSPEED;

			// Mise a jour de la vélocité
			state->ball.velocity.x = x;
			state->ball.velocity.y = -y;
		// Si la balle est tombée
		} else if (state->ball.pos.y > ctx->height) {
			// Fin de la partie, passage a l'écran du score
			SetState(STATE_SCORE);
		}
	}

	/* Vérification de sécurité pour s'assurer que h et w sont valides
	 * (elle ne le sont pas forcement si le contexte n'est pas totalement initialisé */
	// TODO: Prendre en compte le rayon de la balle
	if(h != 0 && w != 0) {
		/* Détermination de la brique avec laquelle la balle entre en
		 * collision dans sa position actuelle */
		int brickX = state->ball.pos.x / w, brickY = state->ball.pos.y / h,
			lastBrickX = lastX / w, lastBrickY = lastY / h;

		// Si cette brique existe
		if(state->bricks[brickX][brickY] != BRICK_NONE) {
			// Détruire la brique en fonction de son type
			switch(state->bricks[brickX][brickY]) {
				case BRICK_ONETOUCH:
					state->bricks[brickX][brickY] = BRICK_NONE;
					state->brickCount--;
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

			// TODO: Ajouter des drops

			// Rebond de la balle en fonction de la face touchée de la brique
			if(brickX > lastBrickX)
				state->ball.velocity.x = -fabs(state->ball.velocity.x);

			if(brickX < lastBrickX)
				state->ball.velocity.x = fabs(state->ball.velocity.x);

			if(brickY > lastBrickY)
				state->ball.velocity.y = -fabs(state->ball.velocity.y);

			if(brickY < lastBrickY)
				state->ball.velocity.y = fabs(state->ball.velocity.y);
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

