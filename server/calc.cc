// Copyright 2014 Huns de Troyes
#include "include/calc.h"

void MouseLocked(void* user_data, int32_t result) {
  PostNumber(result);
}

// Initialise l'état de la partie
void GameInit(PSContext2D_t* ctx, Jeu* state) {
	PSInstance::GetInstance()->SetEnabledEvents(PSE_ALL);
	PSInstance::GetInstance()->RequestInputEvents(PP_INPUTEVENT_CLASS_MOUSE |
												  PP_INPUTEVENT_CLASS_KEYBOARD |
												  PP_INPUTEVENT_CLASS_TOUCH);

	srand(time(NULL));

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

	for(i = 0; i < MAXDROP; i++) {
		state->drops[i].pos = PP_MakeFloatPoint(0, 0);
		state->drops[i].type = DROP_NONE;
	}

	state->ball.pos = PP_MakeFloatPoint(1100/2, 700 - 20);
	float x = sin((PI / 180) * STARTANGLE) * BALLSPEED,
		y = cos((PI / 180) * STARTANGLE) * BALLSPEED;
	state->ball.velocity = PP_MakeFloatPoint(x, y);
	state->ball.radius = 5;

	state->paddle = PP_MakeRectFromXYWH(0, 0, 100, 20);
}

float clamp(float val, float low, float high) {
	return fmin(fmax(val, low), high);
}

// Gère les évenements
void GameHandleEvent(PSEvent* event, Jeu* state, PSContext2D_t* ctx) {
	PPB_InputEvent* pInputEvent = (PPB_InputEvent*)
		PSGetInterface(PPB_INPUT_EVENT_INTERFACE);
	PPB_KeyboardInputEvent* pKeyboardInput = (PPB_KeyboardInputEvent*)
		PSGetInterface(PPB_KEYBOARD_INPUT_EVENT_INTERFACE);
	PPB_MouseInputEvent* pMouseInput = (PPB_MouseInputEvent*)
		PSGetInterface(PPB_MOUSE_INPUT_EVENT_INTERFACE);

	if (event->type == PSE_INSTANCE_HANDLEINPUT) {
		switch (pInputEvent->GetType(event->as_resource)) {
			case PP_INPUTEVENT_TYPE_MOUSEDOWN: {
				PPB_MouseLock* pMouseLock = (PPB_MouseLock*)
					PSGetInterface(PPB_MOUSELOCK_INTERFACE);
				pMouseLock->LockMouse(PSGetInstanceId(),
									  PP_MakeCompletionCallback(MouseLocked,
																0));
				break;
			}
			case PP_INPUTEVENT_TYPE_KEYDOWN: {
				uint32_t key_code = pKeyboardInput->GetKeyCode(event->as_resource);
				// PostMessage("%d", key_code);
				switch(key_code) {
						case 39 :
							state->paddle.point.x += ctx->width / 50;
							break;
						case 37 :
							state->paddle.point.x -= ctx->width / 50;
							break;
				}
				break;
			}
			case PP_INPUTEVENT_TYPE_MOUSEMOVE: {
				struct PP_Point movement = pMouseInput->GetMovement(event->as_resource);
				state->paddle.point.x += movement.x;
				break;
			}
			default:
				PostNumber(event->type);
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
	int h = ctx->height / BRICKH, w = ctx->width / BRICKW,
		lastX = clamp(state->ball.pos.x, 1, ctx->width),
		lastY = clamp(state->ball.pos.y, 1, ctx->height);

	state->paddle.point.x = clamp(state->paddle.point.x, 0,
								  ctx->width - state->paddle.size.width);

	state->ball.pos.x += state->ball.velocity.x;
	state->ball.pos.y += state->ball.velocity.y;

	if(state->ball.pos.x - state->ball.radius <= 0) {
		state->ball.pos.x = state->ball.radius + 1;
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

	if(state->ball.pos.y + state->ball.radius>
	   ctx->height - state->paddle.size.height &&
	   state->ball.pos.x >= state->paddle.point.x &&
	   state->ball.pos.x <= state->paddle.point.x + state->paddle.size.width) {
		state->ball.pos.y = ctx->height - (
			state->paddle.size.height + state->ball.radius + 1);
		float angle = ((90 * (
			(state->ball.pos.x - state->paddle.point.x) /
			state->paddle.size.width)) / 2) - (45 / 2),
			x = sin((PI / 180) * angle) * BALLSPEED,
			y = cos((PI / 180) * angle) * BALLSPEED;
		state->ball.velocity.x = x;
		state->ball.velocity.y = -y;
	}

	// TODO: Prendre en compte le rayon de la balle
	if(h != 0 && w != 0) {
		int brickX = state->ball.pos.x / w, brickY = state->ball.pos.y / h,
			lastBrickX = lastX / w, lastBrickY = lastY / h;

		if(state->bricks[brickX][brickY] != BRICK_NONE) {
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

	if(state->brickCount <= 0)
		SetState(STATE_SCORE);
}

// Determine la distance entre 2 points
float Dist(struct PP_FloatPoint from, struct PP_FloatPoint to) {
	return sqrt(pow(from.x - to.x, 2) + pow(from.y - to.y, 2));
}

