#include "jeu.h"

int server_main(int argc, char* argv[]) {
	PSContext2D_t* context = PSContext2DAllocate(PSContext2DGetNativeImageDataFormat());
	Jeu gamestate;

	Init(&gamestate);

	while(1) {
		PSEvent* event;
		event = PSEventTryAcquire()
		HandleEvent(event);
		PSEventRelease(event);

		PSContext2DGetBuffer(context);
		Draw(context);
		PSContext2DSwapBuffer(context);
	}

	PSContext2DFree(context);
	return 0;
}

void Init(Jeu* state) {
	// TODO: Initialisation
}

void HandleEvent (PSEvent* ps_event) {
	if (0 != PSContext2DHandleEvent(context, ps_event))
		return;
	if (ps_event->type == PSE_INSTANCE_HANDLEINPUT) {
		// FIXME: Convert C event to C++
		pp::InputEvent event(ps_event->as_resource);
		switch (event.GetType()) {
			case PP_INPUTEVENT_TYPE_KEYDOWN: {
				pp::KeyboardInputEvent key(event);
				uint32_t key_code = key.GetKeyCode();
				switch(key_code) {
					// TODO: Handle keypress event
				}
				break;
			}
			case PP_INPUTEVENT_TYPE_MOUSEDOWN:
			case PP_INPUTEVENT_TYPE_MOUSEMOVE: {
				pp::MouseInputEvent mouse = pp::MouseInputEvent(event);
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
