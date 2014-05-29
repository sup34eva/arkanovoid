#include "jeu.h"

// Fonction main du programme
int server_main(int argc, char* argv[]) {
	PSContext2D_t* context = PSContext2DAllocate(PP_IMAGEDATAFORMAT_BGRA_PREMUL);
	Jeu gamestate;

	// Charge les interfaces
	g_pInputEvent =
		(PPB_InputEvent*) PSGetInterface(PPB_INPUT_EVENT_INTERFACE);
	g_pKeyboardInput = (PPB_KeyboardInputEvent*)
		PSGetInterface(PPB_KEYBOARD_INPUT_EVENT_INTERFACE);
	g_pMouseInput =
		(PPB_MouseInputEvent*) PSGetInterface(PPB_MOUSE_INPUT_EVENT_INTERFACE);

	Init(context, &gamestate);

	while (gamestate.brickCount > 0) {
		
			PSEvent* event;

			// TODO: Activer la reception d'evenements
			while ((event = PSEventTryAcquire()) != NULL) {
				PSContext2DHandleEvent(context, event);
				PSEventRelease(event);
			}

			Calc(context, &gamestate);

			if (context->bound) {
				Draw(context, gamestate);
			}
	}

	PSContext2DFree(context);
	return 0;
}

void PostNumber(int32_t num) {
	PP_Var var = PP_MakeDouble(num);
	PSInterfaceMessaging()->PostMessage(PSGetInstanceId(), var);
}

// Determine la distance entre 2 points
int32_t Dist (struct PP_FloatPoint from, struct PP_FloatPoint to) {
	return sqrt(pow(from.x - to.x, 2) + pow(from.y - to.y, 2));
}

// Initialise l'état de la partie
void Init(PSContext2D_t* ctx, Jeu* state) {
	PSEventSetFilter(PSE_ALL);

	srand(6841);

	state->ball.pos = PP_MakeFloatPoint(0, 0);
	state->ball.velocity = PP_MakeFloatPoint(2, 3);
	state->ball.radius = 5;

	state->paddle = PP_MakeRectFromXYWH(0, 0, 100, 20);

	int i, j;
	state->brickCount = 0;
	for(i = 0; i < BRICKW; i++) {
		for(j = 0; j < BRICKH; j++) {
			if(j < BRICKH - 2)
				switch(rand() % 5) {
					case 0:
						state->bricks[i][j] = BRICK_NONE;
						break;
					case 1:
						state->bricks[i][j] = BRICK_ONETOUCH;
						state->brickCount++;
						break;
					case 2:
						state->bricks[i][j] = BRICK_TWOTOUCH;
						state->brickCount++;
						break;
					case 3:
						state->bricks[i][j] = BRICK_THREETOUCH;
						state->brickCount++;
						break;
					case 4:
						state->bricks[i][j] = BRICK_UBER;
				}
			else
				state->bricks[i][j] = BRICK_NONE;
		}
	}

	for(i = 0; i < MAXDROP; i++) {
		state->drops[i].pos = PP_MakeFloatPoint(0, 0);
		state->drops[i].type = DROP_NONE;
	}

}

// Gère les évenements
void HandleEvent(PSEvent* event, Jeu* state) {
	if (event->type == PSE_INSTANCE_HANDLEINPUT) {
		switch (g_pInputEvent->GetType(event->as_resource)) {
			case PP_INPUTEVENT_TYPE_KEYDOWN: {
				uint32_t key_code = g_pKeyboardInput->GetKeyCode(event->as_resource);
				PostNumber(key_code);
				switch(key_code) {
					// TODO: Gestion des touches
				}
				break;
			}
			case PP_INPUTEVENT_TYPE_MOUSEMOVE: {
				struct PP_Point movement = g_pMouseInput->GetMovement(event->as_resource);
				PostNumber(movement.x);
				PostNumber(movement.y);
				//TODO: Gestion de la souris
				break;
			}
			default:
				break;
		}
	}
}

// Determine si un point est contenu dans un rectangle
int Contains(struct PP_Rect rect, PP_FloatPoint point) {
  return (point.x >= rect.point.x) && (point.x < (rect.point.x + rect.size.width)) &&
         (point.y >= rect.point.y) && (point.x < (rect.point.y + rect.size.height));
}

float clamp(float val, float low, float high) {
	return fmin(fmax(val, low), high);
}

// Met a jour l'état du jeu
void Calc(PSContext2D_t* ctx, Jeu* state) {
	int h = ctx->height / BRICKH, w = ctx->width / BRICKW,
		lastX = clamp(state->ball.pos.x, 1, ctx->width), lastY = clamp(state->ball.pos.y, 1, ctx->height);

	state->ball.pos.x += state->ball.velocity.x;
	state->ball.pos.y += state->ball.velocity.y;

	if(state->ball.pos.x - state->ball.radius <= 0) {
		state->ball.pos.x = state->ball.radius + 1;
		state->ball.velocity.x = abs(state->ball.velocity.x);
	}

	if(state->ball.pos.x + state->ball.radius >= ctx->width) {
		state->ball.pos.x = ctx->width - state->ball.radius - 1;
		state->ball.velocity.x = -abs(state->ball.velocity.x);
	}

	if(state->ball.pos.y - state->ball.radius <= 0) {
		state->ball.pos.y = state->ball.radius + 1;
		state->ball.velocity.y = abs(state->ball.velocity.y);
	}

	if(state->ball.pos.y + state->ball.radius >= ctx->height - state->paddle.size.height) {
		state->ball.pos.y = ctx->height - (state->paddle.size.height + state->ball.radius + 1);
		state->ball.velocity.y = -abs(state->ball.velocity.y);
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

			//TODO: Ajouter des drops

			if(brickX > lastBrickX)
				state->ball.velocity.x = -abs(state->ball.velocity.x);

			if(brickX < lastBrickX)
				state->ball.velocity.x = abs(state->ball.velocity.x);

			if(brickY > lastBrickY)
				state->ball.velocity.y = -abs(state->ball.velocity.y);

			if(brickY < lastBrickY)
				state->ball.velocity.y = abs(state->ball.velocity.y);
		}
	}
}

// Dessine un rectangle dans un contexte
void DrawRect(PSContext2D_t* ctx, struct PP_Rect rect, uint32_t color) {
	int32_t px, py, right, bottom;

	for(px = clamp(rect.point.x, 0, ctx->width),
		right = clamp(rect.point.x + rect.size.width, 0, ctx->width); px < right; px++) {
		for(py = clamp(rect.point.y, 0, ctx->height),
			bottom = clamp(rect.point.y + rect.size.height, 0, ctx->height); py < bottom; py++) {
			ctx->data[ctx->width * py + px] = color;
		}
	}
}

// Dessine un cercle dans un contexte
void DrawCircle(PSContext2D_t* ctx, struct PP_FloatPoint center, int32_t radius, uint32_t color) {
	int32_t px, py,
	right = clamp(center.x + radius, 0, ctx->width),
	bottom = clamp(center.y + radius, 0, ctx->width);

	for(px = clamp(center.x - radius, 0, ctx->width); px < right; px++) {
		for(py = clamp(center.y - radius, 0, ctx->height); py < bottom; py++) {
			if(Dist(PP_MakeFloatPoint(px, py), center) < radius)
				ctx->data[ctx->width * py + px] = color;
		}
	}
}

// Dessine une frame
void Draw (PSContext2D_t* ctx, Jeu state) {
	PSContext2DGetBuffer(ctx);

	if (NULL == ctx->data)
		return;
	
	DrawRect(ctx, PP_MakeRectFromXYWH(0, 0, ctx->width, ctx->height), COLOR_BLUE);

	int i, j;
	
	// Briques
	for(i = 0; i < BRICKW; i++) {
		for(j = 0; j < BRICKH; j++) {
			if(state.bricks[i][j] != BRICK_NONE)
				DrawRect(ctx, PP_MakeRectFromXYWH(i * (ctx->width / BRICKW), j * (ctx->height / BRICKH), ctx->width / BRICKW, ctx->height / BRICKH), state.bricks[i][j]);
		}
	}
	
	// Drops
	for(i = 0; i < MAXDROP; i++) {
		if(state.drops[i].type != DROP_NONE)
			DrawCircle(ctx, state.drops[i].pos, 10, COLOR_GREEN);
	}
	
	// Paddle
	state.paddle.point.x = state.ball.pos.x - ((state.paddle.size.width / 2) + (state.ball.radius / 2));
	state.paddle.point.y = ctx->height - state.paddle.size.height;
	DrawRect(ctx, state.paddle, COLOR_WHITE);

	// Balle
	DrawCircle(ctx, state.ball.pos, state.ball.radius, COLOR_WHITE);
	
	PSContext2DSwapBuffer(ctx);
}

#ifndef SEL_LDR
PPAPI_SIMPLE_REGISTER_MAIN(server_main)
#endif
