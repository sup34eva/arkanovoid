// Copyright 2014 Huns de Troyes
#include "include/main.h"

State newState = STATE_TITLE;

// Fonction main du programme
int server_main(int argc, char* argv[]) {
	PSContext2D_t* context = PSContext2DAllocate(PP_IMAGEDATAFORMAT_BGRA_PREMUL);
	Jeu game;
	State state = STATE_NONE;

	while (1) {
			if(newState != state) {
				state = newState;
				// PostNumber(state);
				switch(state) {
					case STATE_INGAME:
						GameInit(context, &game);
						break;
					case STATE_TITLE:
						TitleInit();
						break;
					default:
						break;
				}
			}

			PSEvent* event;
			while ((event = PSEventTryAcquire()) != NULL) {
				PSContext2DHandleEvent(context, event);

				switch(state) {
					case STATE_INGAME:
						GameHandleEvent(event, &game, context);
						break;
					case STATE_TITLE:
					case STATE_PAUSED:
					case STATE_SCORE:
						TitleHandleEvent(event, &game, context);
						break;
					default:
						break;
				}

				PSEventRelease(event);
			}

			if (context->bound) {
				PSContext2DGetBuffer(context);

				if (NULL == context->data)
					break;

				switch(state) {
					case STATE_INGAME:
						GameCalc(context, &game);
						GameDraw(context, &game);
						break;
					case STATE_TITLE:
					case STATE_PAUSED:
					case STATE_SCORE:
						TitleDraw(context, COLOR_BLUE);
						break;
					default:
						break;
				}

				PSContext2DSwapBuffer(context);
			}
	}

	PSContext2DFree(context);
	return 0;
}

void PostMessage(const char *format, ...) {
	va_list ap;
	char buffer[100];
	va_start(ap, format);
	vsnprintf(buffer, sizeof buffer, format, ap);
	va_end(ap);
	PSInterfaceMessaging()->PostMessage(PSGetInstanceId(),
										PSInterfaceVar()->VarFromUtf8(buffer,
																	  sizeof buffer));
}

void PostNumber(double num) {
	PSInterfaceMessaging()->PostMessage(PSGetInstanceId(), PP_MakeDouble(num));
}

void SetState(State nw) {
	newState = nw;
}

#ifndef SEL_LDR
PPAPI_SIMPLE_REGISTER_MAIN(server_main)
#endif
