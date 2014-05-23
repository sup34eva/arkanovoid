#include "jeu.h"

int server_main(int argc, char* argv[]) {
	PSContext2D_t* context = PSContext2DAllocate(PSContext2DGetNativeImageDataFormat());
	Jeu gamestate;

	Init(&gamestate);

	while(1) {
		PSEvent* event;
		while ((event = PSEventTryAcquire()) != NULL) {
			HandleEvent(event);
			PSEventRelease(event);
		}

		PSContext2DGetBuffer(context);
		Draw(context);
		PSContext2DSwapBuffer(context);
	}

	PSContext2DFree(context);
	return 0;
}

void Init(Jeu* state) {
	PSEventSetFilter(PSE_ALL);
	// TODO: Initialisation
}

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

void Calc(Jeu* state) {
	// TODO: Update ball
	// TODO: Check collision
}

void Draw (PSContext2D_t* context) {
	// TODO: Rendu
	// NOTE: PaintImageData(context->image, PP_Resource image_data, const struct PP_Point* top_left, const struct PP_Rect* src_rect)
}

#ifndef SEL_LDR
PPAPI_SIMPLE_REGISTER_MAIN(server_main)
#endif
