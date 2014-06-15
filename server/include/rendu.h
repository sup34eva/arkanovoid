// Copyright 2014 Huns De Troyes

#ifndef SERVER_INCLUDE_RENDU_H_
#define SERVER_INCLUDE_RENDU_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <math.h>
#include <inttypes.h>

#include "types.h"
#include "nacl.h"
#include "calc.h"

void LoadTitleTextures(Jeu* state);
void LoadGameTextures(Jeu* state);
Texture LoadTexture(const char* url);
void TitleDraw(PSContext2D_t* ctx,
			  Jeu* state);
void GameDraw(PSContext2D_t* ctx,
			  Jeu* state);
void PauseDraw(PSContext2D_t* ctx,
			   Jeu* state);
void ScoreDraw(PSContext2D_t* ctx,
			   Jeu* state);
void DrawTexture(PSContext2D_t* ctx,
				 struct PP_Point origin,
				 const Texture tex);
void DrawCircle(PSContext2D_t* ctx,
				struct PP_FloatPoint center,
				int32_t radius,
				uint32_t color);
void DrawRect(PSContext2D_t* ctx,
			  struct PP_Rect rect,
			  uint32_t color);
void DrawText(PSContext2D_t* ctx,
			  struct PP_Point origin,
			  const char* text,
			  const Texture font);
char* IntToText(uint64_t num);

#endif  // SERVER_INCLUDE_RENDU_H_
