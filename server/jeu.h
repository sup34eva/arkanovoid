// Copyright 2014 Huns De Troyes

#ifndef SERVER_JEU_H_
#define SERVER_JEU_H_

#include <stdio.h>
#include <math.h>

// Charge les images
#include "paddle.h"

// Charge l'API simple
#include "ppapi_simple/ps.h"
#include "ppapi_simple/ps_context_2d.h"
#include "ppapi_simple/ps_event.h"
#include "ppapi_simple/ps_interface.h"
#include "ppapi_simple/ps_main.h"
#include "ppapi_simple/ps_instance.h"

#ifdef WIN32
#undef PostMessage
#endif

#include "ppapi/c/ppb_input_event.h"
#include "ppapi/c/ppb_mouse_lock.h"

#ifdef SEL_LDR
#define server_main main
#endif

#define PI 3.141592653589793

#define BALLSPEED 3
#define STARTANGLE 180

#define BRICKW 8
#define BRICKH 10
#define MAXDROP 5
#define BALLRADIUS 10

#define COLOR_INVIS	0x00000000
#define COLOR_BLACK	0xff000000
#define COLOR_LGREY	0xffd3d3d3
#define COLOR_GREY	0xff808080
#define COLOR_RED 	0xffff0000
#define COLOR_YELLOW	0xffffff00
#define COLOR_GREEN	0xff00ff00
#define COLOR_BLUE	0xff0000ff
#define COLOR_WHITE	0xffffffff

// Enum des types de briques / drops
typedef enum {
    BRICK_NONE = COLOR_INVIS,
    BRICK_ONETOUCH = COLOR_WHITE,
    BRICK_TWOTOUCH = COLOR_LGREY,
    BRICK_THREETOUCH = COLOR_GREY,
	BRICK_UBER = COLOR_YELLOW
} BrickType;

typedef enum {
    DROP_NONE,
	DROP_PADDLE_PLUS,
    DROP_STICK,
    DROP_CLONE,
    DROP_EXPLODE,
	DROP_SPEED_LESS,
	DROP_PADDLE_LESS,
	DROP_LOSE,
	DROP_SPEED_PLUS
} DropType;

// Structures de base du jeu : les drops, et le jeu en lui même
typedef struct {
	PP_FloatPoint pos;
	DropType type;
} Drop;

typedef struct {
	PP_FloatPoint pos;
	PP_FloatPoint velocity;
	uint32_t radius;
} Ball;

typedef struct {
	Ball ball;
	struct PP_Rect paddle;
	BrickType bricks[BRICKW][BRICKH];
	int brickCount;
	Drop drops[MAXDROP];
} Jeu;

PPB_InputEvent* g_pInputEvent;
PPB_KeyboardInputEvent* g_pKeyboardInput;
PPB_MouseInputEvent* g_pMouseInput;

void Init(PSContext2D_t*, Jeu* state);
void HandleEvent(PSEvent* event, Jeu* state, PSContext2D_t* ctx);
void Calc(PSContext2D_t* ctx, Jeu* state);
void Draw(PSContext2D_t* ctx, Jeu state);

static const PPB_MouseLock* s_MouseLock;
const PPB_MouseLock* PSInterfaceMouseLock() { return s_MouseLock; }

#endif  // SERVER_JEU_H_
