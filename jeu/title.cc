// Copyright 2014 Huns de Troyes
#include "include/title.h"

// Initialisation de l'écran titre
void TitleInit(Jeu* state) {
	/* Définitions des événements que le programme doit recevoir :
	 * (souris, clavier et écran tactile) */
	PSInstance::GetInstance()->SetEnabledEvents(PSE_ALL);
	PSInstance::GetInstance()->RequestInputEvents(PP_INPUTEVENT_CLASS_MOUSE |
												  PP_INPUTEVENT_CLASS_KEYBOARD |
												  PP_INPUTEVENT_CLASS_TOUCH);

	// Chargement des textures
	LoadTitleTextures(state);

	// Interface de verrouillage du curseur
	PPB_MouseLock* pMouseLock = (PPB_MouseLock*)
		PSGetInterface(PPB_MOUSELOCK_INTERFACE);
	// Déverrouille le curseur
	pMouseLock->UnlockMouse(PSGetInstanceId());
}

// Gestion des evenements sur l'écran titre
void TitleHandleEvent(PSEvent* event, Jeu* state, PSContext2D_t* ctx) {
	// Interface de gestion des evenements
	PPB_InputEvent* pInputEvent = (PPB_InputEvent*)
		PSGetInterface(PPB_INPUT_EVENT_INTERFACE);
	PPB_MouseInputEvent* pMouseInput = (PPB_MouseInputEvent*)
		PSGetInterface(PPB_MOUSE_INPUT_EVENT_INTERFACE);

	switch(event->type) {
		case PSE_INSTANCE_HANDLEINPUT: {
			switch (pInputEvent->GetType(event->as_resource)) {
				case PP_INPUTEVENT_TYPE_MOUSEUP: {  // Click de souris
					struct PP_Point pos = pMouseInput->GetPosition(event->as_resource),
					origin;
					struct PP_Size size = PP_MakeSize(state->textures[1].width,
													  state->textures[1].height);

					if(state->state == STATE_TITLE) {
						origin = PP_MakePoint((ctx->width / 2) - (state->textures[1].width / 2),
											  state->textures[0].height);
					} else {
						origin = PP_MakePoint((ctx->width / 2) - (state->textures[1].width / 2),
											  state->textures[0].height + state->textures[21].height);
					}

					if(pos.x >= origin.x &&
					   pos.y >= origin.y &&
					   pos.x <= origin.x + size.width &&
					   pos.y <= origin.y + size.height) {
						// Interface de verrouillage du curseur
						PPB_MouseLock* pMouseLock = (PPB_MouseLock*)
							PSGetInterface(PPB_MOUSELOCK_INTERFACE);
						// Verrouille le curseur pour améliorer l'immersion / ergonomie
						pMouseLock->LockMouse(PSGetInstanceId(),
											  PP_MakeCompletionCallback(MouseLocked, state));

						// Active un item du shop
						int i;
						state->bonus = 0;
						for(i = 0; i < 9; i++) {
							if(state->shop[i] > 0) {
								state->bonus = state->shop[i];
								PostMessage("delete:%d", state->shop[i]);
								break;
							}
						}

						// Passe a l'état "en jeu"
						state->newState = STATE_INGAME;
					}

					if(state->state == STATE_SCORE) {
						origin = PP_MakePoint((ctx->width / 2) - (state->textures[1].width / 2),
											  state->textures[0].height +
											  state->textures[21].height +
											  state->textures[1].height);
						size = PP_MakeSize(state->textures[1].width,
										   state->textures[1].height);

						if(pos.x >= origin.x &&
						   pos.y >= origin.y &&
						   pos.x <= origin.x + size.width &&
						   pos.y <= origin.y + size.height) {
							PostMessage("score:%d", state->score);
						}
					}
					break;
				}
				default:
					break;
			}
			break;
		}
		case PSE_INSTANCE_HANDLEMESSAGE: {
			PP_Var message = event->as_var;
			if(message.type == PP_VARTYPE_ARRAY) {
				// Interface de lecture des tableaux
				PPB_VarArray* pVarArray = (PPB_VarArray*)
					PSGetInterface(PPB_VAR_ARRAY_INTERFACE);
				// Longeur du message
				uint32_t i, len = pVarArray->GetLength(message);
				for(i = 0; i < len; i++) {
					PP_Var item = pVarArray->Get(message, i);
					if(item.type == PP_VARTYPE_INT32) {
						state->shop[i] = item.value.as_int;
					}
				}
			}
			break;
		}
		default:
			break;
	}
}

// Fonction appelée a chaque frame de l'écran titre pour dessiner l'image
void TitleDraw(PSContext2D_t* ctx, Jeu* state) {
	DrawTexture(ctx, PP_MakePoint(0, 0), state->textures[8]);

	// Affichage du logo
	PP_Point origin = PP_MakePoint((ctx->width / 2)
								   - (state->textures[0].width / 2), 0);
	DrawTexture(ctx, origin, state->textures[0]);

	// Affichage du bouton "nouvelle partie"
	origin = PP_MakePoint((ctx->width / 2) -
						  (state->textures[1].width / 2),
						  state->textures[0].height);
	DrawTexture(ctx, origin, state->textures[1]);


	// Interface de lecture des tableaux
	int i;
	for(i = 0; i < 9; i++) {
		Texture tex;
		PP_Point pos = PP_MakePoint(i * 20, ctx->height - 20);
		switch(state->shop[i]) {
			case 3:
				tex = state->textures[9];
				break;
			case 4:
				tex = state->textures[13];
				break;
			case 5:
			case 6:
				tex = state->textures[10];
				break;
			case 7:
			case 8:
				tex = state->textures[12];
				break;
			case 9:
			case 10:
				tex = state->textures[11];
				break;
			default:
				tex = state->textures[4];
				break;
		}
		DrawTexture(ctx, pos, tex);
	}
}

// Fonction appelée a chaque frame de l'écran titre pour dessiner l'image
void ScoreDraw(PSContext2D_t* ctx, Jeu* state) {
	DrawTexture(ctx, PP_MakePoint(0, 0), state->textures[8]);

	// Affichage du logo
	PP_Point origin = PP_MakePoint((ctx->width / 2)
								   - (state->textures[0].width / 2), 0);
	DrawTexture(ctx, origin, state->textures[0]);

	// Affichage du score
	char score[20];
	snprintf(score, sizeof score, "%" PRIu64, state->score);
	origin = PP_MakePoint((ctx->width / 2) -
		((state->textures[22].width + (strlen(score) * 26)) / 2)
		, state->textures[0].height);
	DrawTexture(ctx, origin, state->textures[22]);
	origin.x += state->textures[22].width;
	DrawText(ctx, origin, score, state->textures[21]);

	// Affichage du bouton "nouvelle partie"
	origin = PP_MakePoint((ctx->width / 2) -
						  (state->textures[1].width / 2),
						  state->textures[0].height + state->textures[21].height);
	DrawTexture(ctx, origin, state->textures[1]);

	// Affichage du bouton "partager"
	origin = PP_MakePoint((ctx->width / 2) -
						  (state->textures[1].width / 2),
						  state->textures[0].height +
						  state->textures[21].height +
						  state->textures[1].height);
	DrawTexture(ctx, origin, state->textures[2]);
}
