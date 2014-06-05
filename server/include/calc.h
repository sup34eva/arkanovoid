// Copyright 2014 Huns De Troyes

#ifndef SERVER_INCLUDE_CALC_H_
#define SERVER_INCLUDE_CALC_H_

#include <math.h>
#include "types.h"
#include "nacl.h"

// FIXME: Test header
#include "main.h"

void Init(PSContext2D_t*, Jeu* state);
float clamp(float val, float low, float high);
void HandleEvent(PSEvent* event, Jeu* state, PSContext2D_t* ctx);
int Contains(struct PP_Rect rect, PP_FloatPoint point);
void Calc(PSContext2D_t* ctx, Jeu* state);
float Dist(struct PP_FloatPoint from, struct PP_FloatPoint to);

#endif  // SERVER_INCLUDE_CALC_H_
