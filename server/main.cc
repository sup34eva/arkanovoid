// Copyright 2014 Huns de Troyes
#include "include/main.h"

// Fonction main du programme
int server_main(int argc, char* argv[]) {
	PSContext2D_t* context = PSContext2DAllocate(PP_IMAGEDATAFORMAT_BGRA_PREMUL);
	Jeu gamestate;

	Init(context, &gamestate);

	while (gamestate.brickCount > 0) {
			PSEvent* event;

			// TODO: Activer la reception d'evenements
			while ((event = PSEventTryAcquire()) != NULL) {
				PSContext2DHandleEvent(context, event);
				HandleEvent(event, &gamestate, context);
				PSEventRelease(event);
			}

			if (context->bound) {
				Calc(context, &gamestate);
				Draw(context, gamestate);
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

#ifndef SEL_LDR
PPAPI_SIMPLE_REGISTER_MAIN(server_main)
#endif
