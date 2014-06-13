// Copyright 2014 Huns de Troyes
#include "include/main.h"

// Cette variable stocke l'état du programme a la prochaine frame
State newState = STATE_TITLE;

// Fonction main du programme
int server_main(int argc, char* argv[]) {
	// Allocation d'un contexte 2d pour dessiner les images du jeu dans un cadre
	PSContext2D_t* context = PSContext2DAllocate(PP_IMAGEDATAFORMAT_BGRA_PREMUL);
	// Création des données de jeu
	Jeu game;
	// Le jeu est initialisé a l'état none pour que la fonction TitleInit s'execute
	State state = STATE_NONE;

	// Boucle de jeu
	while (1) {
		// Si l'état a changé depuis la dernière frame
		if(newState != state) {
			// Change l'etat du jeu
			state = newState;
			// Initialise le nouvel etat
			switch(state) {
				case STATE_INGAME:
					GameInit(context, &game);
					break;
				case STATE_TITLE:
				case STATE_SCORE:
					TitleInit(&game);
					break;
				default:
					break;
			}
		}

		// Prise en charge des evenements
		PSEvent* event;
		// Pour chaque nouvel evenement
		while ((event = PSEventTryAcquire()) != NULL) {
			// Envoie de l'evenent au contexte si besoin est
			PSContext2DHandleEvent(context, event);

			// Appel la fonction de gestion d'event en fonction de l'etat
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

			// Liberation de la memoire allouée a l'evenement
			PSEventRelease(event);
		}

		// Si le contexte est initialisé
		if (context->bound) {
			// Création du buffer pour stocker l'image temporairement pendant son calcul
			PSContext2DGetBuffer(context);

			// Saut de la frame si le buffer n'existe pas
			if (NULL == context->data)
				break;

			// Mise a jour de du jeu et calcul de l'image en fonction de l'état
			switch(state) {
				case STATE_INGAME:
					GameCalc(context, &game);
					GameDraw(context, &game);
					break;
				case STATE_TITLE:
				case STATE_SCORE:
					TitleDraw(context, &game);
					break;
				case STATE_PAUSED:
					PauseDraw(context, &game);
					break;
				default:
					break;
			}

			// Envoi du buffer pour affichage de l'image a l'écran
			PSContext2DSwapBuffer(context);
		}
	}

	// Libération de la mémoire alouée au contexte
	PSContext2DFree(context);
	return 0;
}

// Fonctions de debug
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

// Change l'état du programme
void SetState(State nw) {
	newState = nw;
}

// Enresitre la fonction server_main en tant que fonction main auprès d'NaCl
#ifndef SEL_LDR
PPAPI_SIMPLE_REGISTER_MAIN(server_main)
#endif
