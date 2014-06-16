// Copyright 2014 Huns de Troyes
#include "include/calc.h"

void NewLife(PSContext2D_t* ctx, Jeu* state) {
	// Mise a zero des drop (type = none)
	int i, numBall;
	for(i = 0; i < MAXDROP; i++) {
		state->drops[i].pos = PP_MakeFloatPoint(0, 0);
		state->drops[i].type = DROP_NONE;
	}

	if(state->bonus == 10 || (state->bonus == 9 && state->lives == 5))
		numBall = 2;
	else
		numBall = 1;

	// Position initiale de la balle
	for(i = 0; i < MAXBALL; i++) {
        InitBall(state, i);
        if (i < numBall) {
            if (state->bonus == 8 || (state->bonus == 7 && state->lives == 5))
				state->ball[i].type = BALL_EXPLODE;
			else
				state->ball[i].type = BALL_CLASSIC;
			state->ball[i].stuck = PP_TRUE;
			state->ball[i].speed = BALLSPEED;
			state->ball[i].velocity = PP_MakeFloatPoint(0, 0);
		} else {
            state->ball[i].type = BALL_NONE;
		}
	}
	state->ballCount = 1;

	// Position initiale de la paddle
	if(state->bonus == 3)
		state->paddle.surf = PP_MakeRectFromXYWH(1100/2 - 50, 0, MAXPADDLE,
												 state->textures[6].height);
	else
		state->paddle.surf = PP_MakeRectFromXYWH(1100/2 - 50, 0, BASEPADDLE,
												 state->textures[6].height);
	if(state->bonus == 6 || (state->bonus == 5 && state->lives == 5))
		state->paddle.sticky = PP_TRUE;
	else
		state->paddle.sticky = PP_FALSE;
}

// Appelée quand la souris est vérrouillée
void MouseLocked(void* data, int32_t result) {
	if(result == PP_OK) {
		Jeu* state = (Jeu*) data;
		state->newState = STATE_INGAME;
	} else {
		PostMessage("PointerLock error %d", result);
	}
}

int collides(Ball a, struct PP_Rect b) {
	return a.pos.x - a.radius < b.point.x + b.size.width &&
		a.pos.x + a.radius > b.point.x &&
		a.pos.y - a.radius < b.point.y + b.size.height &&
		a.pos.y + a.radius > b.point.y;
}

void WallsCollision(PSContext2D_t* ctx, Jeu* state, int i) {
	if (state->ball[i].pos.x - state->ball[i].radius <= 0) {
		state->ball[i].pos.x = state->ball[i].radius + 1;
		state->ball[i].velocity.x = fabs(state->ball[i].velocity.x);
	}

	if (state->ball[i].pos.x + state->ball[i].radius >= ctx->width) {
		state->ball[i].pos.x = ctx->width - state->ball[i].radius - 1;
		state->ball[i].velocity.x = -fabs(state->ball[i].velocity.x);
	}

	if (state->ball[i].pos.y - state->ball[i].radius <= 0) {
		state->ball[i].pos.y = state->ball[i].radius + 1;
		state->ball[i].velocity.y = fabs(state->ball[i].velocity.y);
	}
}

void PaddleCollision(PSContext2D_t* ctx, Jeu* state, int i) {
	float angle = ((90 * (
		(state->ball[i].pos.x - state->paddle.surf.point.x) /
		state->paddle.surf.size.width)) / 2) - (45 / 2),
		x = sin(angle / 180 * PI) * state->ball[i].speed,
		y = cos(angle / 180 * PI) * state->ball[i].speed;
	state->ball[i].velocity.x = x;
	state->ball[i].velocity.y = -y;
}

void HitBrick(PSContext2D_t* ctx, Jeu* state, int j, int k) {
	int h = ctx->height / BRICKH,
		w = ctx->width / BRICKW,
		x = j * (ctx->width / BRICKW),
		y = k * (ctx->height / BRICKH);
	switch (state->bricks[j][k]) {
		case BRICK_ONETOUCH:
			state->score++;
			state->bricks[j][k] = BRICK_NONE;
			state->brickCount--;
			if (rand() < (RAND_MAX / 2))
				SpawnDrop(x + (w/2), y + (h/2), state);
			break;
		case BRICK_TWOTOUCH:
			state->bricks[j][k] = BRICK_ONETOUCH;
			break;
		case BRICK_THREETOUCH:
			state->bricks[j][k] = BRICK_TWOTOUCH;
			break;
		default:
			break;
	}
}

void SpawnDrop(int x, int y, Jeu* state) {
    int i;
    for (i = 0; i < MAXDROP; i++)
    if (state->drops[i].type == DROP_NONE) {
        state->drops[i].pos = PP_MakeFloatPoint(x, y);
        switch (rand() % 8) {
            case 0 :
                state->drops[i].type = DROP_PADDLE_PLUS;
                break;
            case 1 :
                state->drops[i].type = DROP_STICK;
                break;
            case 2 :
                state->drops[i].type = DROP_CLONE;
                break;
            case 3 :
                state->drops[i].type = DROP_EXPLODE;
                break;
            case 4 :
                state->drops[i].type = DROP_SPEED_LESS;
                break;
            case 5 :
                state->drops[i].type = DROP_PADDLE_LESS;
                break;
            case 6 :
                state->drops[i].type = DROP_LOSE;
                break;
            case 7 :
                state->drops[i].type = DROP_SPEED_PLUS;
                break;
        }
        return;
    }
}

void InitBall(Jeu* state, int i) {
    if (state->bonus == 4)
		state->ball[i].speed = BALLSPEED;
	else
		state->ball[i].speed = BALLSPEED;
	state->ball[i].velocity = PP_MakeFloatPoint(0, 0);
    state->ball[i].pos = PP_MakeFloatPoint(1100/2, 700 - 20);
    state->ball[i].radius = BALLRADIUS;
	state->ball[i].stuck = PP_FALSE;
}

int AddBall(Jeu* state) {
    int i;
    for (i = 0; i < MAXBALL; i++) {
        if (state->ball[i].type == BALL_NONE) {
            InitBall(state, i);
			state->ball[i].velocity = PP_MakeFloatPoint(0, -state->ball[i].speed);
            state->ballCount++;
            state->ball[i].type = BALL_CLASSIC;
            return i;
        }
    }
    return 0;
}