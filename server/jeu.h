// Copyright 2014 Huns De Troyes

#ifndef SERVER_JEU_H_
#define SERVER_JEU_H_

// Charge l'API simple
#include "ppapi_simple/ps_main.h"
#include "ppapi_simple/ps_context_2d.h"

#ifdef SEL_LDR
#define server_main main
#endif

#define BRICKW 8
#define BRICKH 10
#define MAXDROP 5

typedef struct {
	PP_Rect surf;
	int type;
} Brick;

typedef struct {
	PP_Point pos;
	int type;
} Drop;

typedef struct {
	PP_Point ball;
	PP_Rect paddle;
	Brick bricks[BRICKW * BRICKH];
	Drop drops[MAXDROP];
} Jeu;

void Init(Jeu* state);
void HandleEvent(PSEvent* ps_event);
void Calc(Jeu* state);
void Draw(PSContext2D_t* context);

#endif  // SERVER_JEU_H_
