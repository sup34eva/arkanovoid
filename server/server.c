#include "jeu.h"

// Fonction main du programme
int server_main(int argc, char* argv[]) {
	PSContext2D_t* context = PSContext2DAllocate(PP_IMAGEDATAFORMAT_BGRA_PREMUL);
	Jeu gamestate;

	Init(&gamestate);

	while(1) {
		PSEvent* event;
		while ((event = PSEventTryAcquire()) != NULL) {
			HandleEvent(event);
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
void HandleEvent (PSEvent* event) {
	if (0 != PSContext2DHandleEvent(context, event))
		return;
	if (event->type == PSE_INSTANCE_HANDLEINPUT) {
		switch (GetType(event->as_resource)) {
			case PP_INPUTEVENT_TYPE_KEYDOWN: {
				uint32_t key_code = GetKeyCode(event->as_resource);
				switch(key_code) {
					// TODO: Handle keypress event
				}
				break;
			}
			case PP_INPUTEVENT_TYPE_MOUSEMOVE: {
				PP_Point movement = GetMovement(event->as_resource);
				// TODO: Handle mouse event
				break;
			}
			default:
				break;
		}
	}
}

// Determine si un point est contenu dans un rectangle
int Contains(PP_Rect rect, PP_Point point) {
  return (point.X >= rect.point->X) && (point.X < (rect.point->X + rect.size->width)) &&
         (point.Y >= rect.point->Y) && (point.X < (rect.point->Y + rect.size->height));
}

// Determine la distance entre 2 points
int32_t Dist (PP_Point from, PP_Point to) {
	return sqrt(pow(from.X - to.X, 2) + pow(from.Y - to.Y, 2));
}

// Met a jour l'état du jeu
void Calc(Jeu* state) {
	// TODO: Update ball
	// TODO: Check collision
}

// Dessine un rectangle dans un contexte
void DrawRect(PSContext2D_t* ctx, PP_Rect rect, uint32_t color) {
	int32_t right = rect.point->X + rect.size->width,
			bottom = rect.point->Y + rect.size->height;

	for(int px = rect.point->X; px < right; px++) {
		for(int py = rect.point->Y; py < bottom; py++) {
			ctx->data[ctx->width * py + px] = color;
		}
	}
}

// Dessine un cercle dans un contexte
void DrawCircle(PSContext2D_t* ctx, PP_Point center, int32_t radius, uint32_t color) {
	int32_t x = center.X - radius,
			y = center.Y - radius,
			right = center.X + radius,
			bottom = center.Y + radius;

	for(int px = x; px < right; px++) {
		for(int py = y; py < bottom; py++) {
			if(Dist(MakePoint(px, py), center) < radius)
				ctx->data[ctx->width * py + px] = color;
		}
	}
}

// Dessine une frame
void Draw (PSContext2D_t* context, Jeu state) {
	// TODO: Rendu
}

#ifndef SEL_LDR
PPAPI_SIMPLE_REGISTER_MAIN(server_main)
#endif
