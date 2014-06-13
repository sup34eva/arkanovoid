// Copyright 2014 Huns de Troyes
#ifndef SERVER_INCLUDE_TYPES_H_
#define SERVER_INCLUDE_TYPES_H_

#include "const.h"
#include "colors.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_rect.h"

// Enum des types de briques / drops
typedef enum {
    BRICK_NONE,
    BRICK_ONETOUCH,
    BRICK_TWOTOUCH,
    BRICK_THREETOUCH,
	BRICK_UBER
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

typedef enum {
    STATE_NONE,
	STATE_TITLE,
    STATE_INGAME,
    STATE_PAUSED,
    STATE_SCORE
} State;

typedef enum {
    BALL_NONE,
    BALL_CLASSIC,
    BALL_STICKY,
    BALL_EXPLODE
} BallType;

// Structures de base du jeu : les drops, et le jeu en lui même
typedef struct {
	PP_FloatPoint pos;
	DropType type;
} Drop;

typedef struct {
	PP_FloatPoint pos;
	PP_FloatPoint velocity;
	uint32_t radius;
	BallType type;
	int speed;
} Ball;

typedef struct {
  int  	 width;
  int  	 height;
  int  	 channels; /* 3:RGB, 4:RGBA */
  uint8_t* pixel_data;
} Texture;

typedef struct {
	Ball ball[MAXBALL];
	struct PP_Rect paddle;
	BrickType bricks[BRICKW][BRICKH];
	int brickCount;
	Drop drops[MAXDROP];
	Texture textures[TEXSIZE];
	int lives;
	int ballCount;
} Jeu;

#endif  // SERVER_INCLUDE_TYPES_H_
