// Copyright 2014 Huns De Troyes

#ifndef JEU_INCLUDE_GAME_H_
#define JEU_INCLUDE_GAME_H_

#include "nacl.h"
#include "types.h"
#include "rendu.h"
#include "calc.h"
#include "tools.h"

void GameInit(PSContext2D_t*, Jeu* state);
void GameHandleEvent(PSEvent* event, Jeu* state, PSContext2D_t* ctx);
void GameCalc(PSContext2D_t* ctx, Jeu* state);
void GameDraw(PSContext2D_t* ctx, Jeu* state);

#endif  // JEU_INCLUDE_GAME_H_
