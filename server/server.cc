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

	while (true) {
		PSEvent* event;

		// Consume all available events
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

// Determine la distance entre 2 points
int32_t Dist (struct PP_Point from, struct PP_Point to) {
	return sqrt(pow(from.x - to.x, 2) + pow(from.y - to.y, 2));
}

// Initialise l'état de la partie
void Init(PSContext2D_t* ctx, Jeu* state) {
	PSEventSetFilter(PSE_ALL);

	srand(6841);

	Ball gameBall;
	gameBall.pos = PP_MakePoint(10, 10);
	gameBall.velocity = PP_MakeSize(1, 1);
	gameBall.radius = 5;
	state->ball = gameBall;

	state->paddle = PP_MakeRectFromXYWH(0, 0, 100, 20);

	int i, j;
	for(i = 0; i < BRICKW; i++) {
		for(j = 0; j < BRICKH; j++) {
			if(j < BRICKH - 2)
				switch(rand() % 5) {
					case 0:
						state->bricks[i][j] = BRICK_NONE;
						break;
					case 1:
						state->bricks[i][j] = BRICK_ONETOUCH;
						break;
					case 2:
						state->bricks[i][j] = BRICK_TWOTOUCH;
						break;
					case 3:
						state->bricks[i][j] = BRICK_THREETOUCH;
						break;
					case 4:
						state->bricks[i][j] = BRICK_UBER;
				}
			else
				state->bricks[i][j] = BRICK_NONE;
		}
	}

	for(i = 0; i < MAXDROP; i++) {
		Drop tempDrop;
		tempDrop.pos = PP_MakePoint(0, 0);
		tempDrop.type = DROP_NONE;
		state->drops[i] = tempDrop;
	}

	PP_Var var = PP_MakeDouble(1);
	PSInterfaceMessaging()->PostMessage(PSGetInstanceId(), var);

}

void PostNumber(int32_t num) {
	PP_Var var = PP_MakeDouble(num);
	PSInterfaceMessaging()->PostMessage(PSGetInstanceId(), var);
}

// Gère les évenements
void HandleEvent(PSEvent* event, Jeu* state) {
	if (event->type == PSE_INSTANCE_HANDLEINPUT) {
		switch (g_pInputEvent->GetType(event->as_resource)) {
			case PP_INPUTEVENT_TYPE_KEYDOWN: {
				uint32_t key_code = g_pKeyboardInput->GetKeyCode(event->as_resource);
				PostNumber(key_code);
				switch(key_code) {
					// TODO: Handle keypress event
				}
				break;
			}
			case PP_INPUTEVENT_TYPE_MOUSEMOVE: {
				struct PP_Point movement = g_pMouseInput->GetMovement(event->as_resource);
				PostNumber(movement.x);
				PostNumber(movement.y);
				state->paddle.point.x += movement.x;
				break;
			}
			default:
				break;
		}
	}
}

// Determine si un point est contenu dans un rectangle
int Contains(struct PP_Rect rect, struct PP_Point point) {
  return (point.x >= rect.point.x) && (point.x < (rect.point.x + rect.size.width)) &&
         (point.y >= rect.point.y) && (point.x < (rect.point.y + rect.size.height));
}

// Met a jour l'état du jeu
void Calc(PSContext2D_t* ctx, Jeu* state) {
	state->ball.pos.x += state.ball.velocity.width;
	state->ball.pos.y += state.ball.velocity.height;
	// TODO: Check collision
}

// Dessine un rectangle dans un contexte
void DrawRect(PSContext2D_t* ctx, struct PP_Rect rect, uint32_t color) {
	int32_t px, py, right, bottom;

	for(px = rect.point.x, right = px + rect.size.width; px < right; px++) {
		for(py = rect.point.y, bottom = py + rect.size.height; py < bottom; py++) {
			ctx->data[ctx->width * py + px] = color;
		}
	}
}

// Dessine un cercle dans un contexte
void DrawCircle(PSContext2D_t* ctx, struct PP_Point center, int32_t radius, uint32_t color) {
	int32_t px, py, right = center.x + radius,
			bottom = center.y + radius;

	for(px = center.x - radius; px < right; px++) {
		for(py = center.y - radius; py < bottom; py++) {
			if(Dist(PP_MakePoint(px, py), center) < radius)
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
	state.paddle.point.y = ctx->height - state.paddle.size.height;
	DrawRect(ctx, state.paddle, COLOR_WHITE);

	// Balle
	DrawCircle(ctx, state.ball.pos, state.ball.radius, COLOR_WHITE);
	
	PSContext2DSwapBuffer(ctx);
}

#ifndef SEL_LDR
PPAPI_SIMPLE_REGISTER_MAIN(server_main)
#endif
