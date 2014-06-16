// Copyright 2014 Huns De Troyes

#ifndef JEU_INCLUDE_TITLE_H_
#define JEU_INCLUDE_TITLE_H_

#include "types.h"
#include "nacl.h"
#include "rendu.h"
#include "tools.h"
#include "calc.h"

void TitleInit(Jeu* state);
void TitleHandleEvent(PSEvent* event, Jeu* state, PSContext2D_t* ctx);
void TitleDraw(PSContext2D_t* ctx, Jeu* state);
void PauseDraw(PSContext2D_t* ctx, Jeu* state);
void ScoreDraw(PSContext2D_t* ctx, Jeu* state);

#endif  // JEU_INCLUDE_TITLE_H_
