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

	fprintf(stderr, "Init");
	Init(&gamestate);

	while(1) {
		PSEvent* event;
		while ((event = PSEventTryAcquire()) != NULL) {
			if (PSContext2DHandleEvent(context, event) == 0)
				HandleEvent(event, &gamestate);
			PSEventRelease(event);
		}

		Calc(&gamestate);

		PSContext2DGetBuffer(context);
		Draw(context, gamestate);
		PSContext2DSwapBuffer(context);
	}

	PSContext2DFree(context);
	return 0;
}

// Initialise l'état de la partie
void Init(Jeu* state) {
	PSEventSetFilter(PSE_ALL);
	// TODO: Initialisation
}

// Gère les évenements
void HandleEvent(PSEvent* event, Jeu* state) {
	if (event->type == PSE_INSTANCE_HANDLEINPUT) {
		switch (g_pInputEvent->GetType(event->as_resource)) {
			case PP_INPUTEVENT_TYPE_KEYDOWN: {
				uint32_t key_code = g_pKeyboardInput->GetKeyCode(event->as_resource);
				switch(key_code) {
					// TODO: Handle keypress event
				}
				break;
			}
			case PP_INPUTEVENT_TYPE_MOUSEMOVE: {
				//struct PP_Point movement = g_pMouseInput->GetMovement(event->as_resource);
				// TODO: Handle mouse event
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

// Determine la distance entre 2 points
int32_t Dist (struct PP_Point from, struct PP_Point to) {
	return sqrt(pow(from.x - to.x, 2) + pow(from.y - to.y, 2));
}

// Met a jour l'état du jeu
void Calc(Jeu* state) {
	// TODO: Update ball
	// TODO: Check collision
}

// Dessine un rectangle dans un contexte
void DrawRect(PSContext2D_t* ctx, struct PP_Rect rect, uint32_t color) {
	int32_t px = rect.point.x,
			py = rect.point.y,
			right = rect.point.x + rect.size.width,
			bottom = rect.point.y + rect.size.height;

	for(; px < right; px++) {
		for(; py < bottom; py++) {
			ctx->data[ctx->width * py + px] = color;
		}
	}
}

// Dessine un cercle dans un contexte
void DrawCircle(PSContext2D_t* ctx, struct PP_Point center, int32_t radius, uint32_t color) {
	int32_t px = center.x - radius,
			py = center.y - radius,
			right = center.x + radius,
			bottom = center.y + radius;

	for(; px < right; px++) {
		for(; py < bottom; py++) {
			if(Dist(PP_MakePoint(px, py), center) < radius)
				ctx->data[ctx->width * py + px] = color;
		}
	}
}

// Dessine une frame
void Draw (PSContext2D_t* context, Jeu state) {
	int i;
	// Briques
	for(i = 0; i < BRICKH * BRICKW; i++) {
		if(state.bricks[i].type != BRICK_NONE)
			DrawRect(context, state.bricks[i].surf, state.bricks[i].type);
	}
	// Drops
	for(i = 0; i < MAXDROP; i++) {
		if(state.drops[i].type != DROP_NONE)
			DrawCircle(context, state.drops[i].pos, 10, COLOR_GREEN);
	}
	// Paddle
	DrawRect(context, state.paddle, COLOR_WHITE);
	// Balle
	DrawCircle(context, state.ball.pos, state.ball.radius, COLOR_WHITE);
}

#ifndef SEL_LDR
PPAPI_SIMPLE_REGISTER_MAIN(server_main)
#endif
