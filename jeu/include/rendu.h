// Copyright 2014 Huns De Troyes

#ifndef JEU_INCLUDE_RENDU_H_
#define JEU_INCLUDE_RENDU_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <math.h>
#include <inttypes.h>

#include "types.h"
#include "nacl.h"
#include "tools.h"

void LoadTitleTextures(Jeu* state);
void LoadGameTextures(Jeu* state);
Texture LoadTexture(const char* url);
void DrawLoadingScreen(PSContext2D_t* ctx,
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

#define RGBA(r, g, b, a) (a << 24) | (r << 16) | (g << 8) | b
#define RGB(r, g, b) (255 << 24) | (r << 16) | (g << 8) | b
#define getRed(c) ((c & 0xff0000) >> 16)
#define getGreen(c) ((c & 0xff00) >> 8)
#define getBlue(c) (c & 0xff)
#define getAlpha(c) ((c & 0xff000000) >> 24)

#endif  // JEU_INCLUDE_RENDU_H_
