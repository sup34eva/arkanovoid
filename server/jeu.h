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
#define BALLRADIUS 10

#define INVIS	0x00000000
#define BLACK	0xff000000
#define LGREY	0xffd3d3d3
#define GREY	0xff808080
#define RED 	0xffff0000
#define YELLOW	0xffffff00
#define GREEN	0xff00ff00
#define BLUE	0xff0000ff
#define WHITE	0xffffffff

// Enum des types de briques / drops
typedef enum {
    NONE = INVIS,
    ONETOUCH = WHITE,
    TWOTOUCH = LGREY,
    THREETOUCH = GREY,
	UBER = YELLOW
} BrickType;

typedef enum {
    PADDLEPLUS,
    STICK,
    CLONE,
    EXPLODE,
	SPEEDLESS,
	PADDLELESS,
	LOSE,
	SPEEDPLUS
} DropType;

// Structures de base du jeu : les briques, les drops, et le jeu en lui même
typedef struct {
	PP_Rect surf;
	BrickType type;
} Brick;

typedef struct {
	PP_Point pos;
	DropType type;
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
