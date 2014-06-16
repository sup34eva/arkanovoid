// Copyright 2014 Huns De Troyes

#ifndef JEU_INCLUDE_CALC_H_
#define JEU_INCLUDE_CALC_H_

#include <math.h>
#include "types.h"
#include "rendu.h"

#include "nacl.h"
#include "tools.h"

int Contains(struct PP_Rect rect, PP_FloatPoint point);
void SpawnDrop(int brickX, int brickY, Jeu* state);
void InitBall(Jeu* state, int i);
int AddBall(Jeu* state);
void PaddleCollision(PSContext2D_t* ctx, Jeu* state, int i);
void NewLife(PSContext2D_t* ctx, Jeu* state);
int collides(Ball a, struct PP_Rect b);
void WallsCollision(PSContext2D_t* ctx, Jeu* state, int i);
void HitBrick(PSContext2D_t* ctx, Jeu* state, int j, int k);
void MouseLocked(void* data, int32_t result);

#endif  // JEU_INCLUDE_CALC_H_
