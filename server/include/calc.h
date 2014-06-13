// Copyright 2014 Huns De Troyes

#ifndef SERVER_INCLUDE_CALC_H_
#define SERVER_INCLUDE_CALC_H_

#include <math.h>
#include "types.h"
#include "nacl.h"
#include "rendu.h"

// FIXME: Test header
#include "main.h"

void TitleInit(Jeu* state);
void GameInit(PSContext2D_t*, Jeu* state);

float clamp(float val, float low, float high);

void GameHandleEvent(PSEvent* event, Jeu* state, PSContext2D_t* ctx);
void TitleHandleEvent(PSEvent* event, Jeu* state, PSContext2D_t* ctx);

int Contains(struct PP_Rect rect, PP_FloatPoint point);
void GameCalc(PSContext2D_t* ctx, Jeu* state);
float Dist(struct PP_FloatPoint from, struct PP_FloatPoint to);
void SpawnDrop(int brickX, int brickY, Jeu* state);
void InitBall(Jeu* state, int i);
int AddBall(Jeu* state);

#endif  // SERVER_INCLUDE_CALC_H_
