// Copyright 2014 Huns de Troyes
#include "include/calc.h"

// Une nouvelle vie commence
void NewLife(PSContext2D_t* ctx, Jeu* state) {
	// Mise a zero des drop (type = none)
	int i, numBall;
	for(i = 0; i < MAXDROP; i++) {
		state->drops[i].pos = PP_MakeFloatPoint(0, 0);
		state->drops[i].type = DROP_NONE;
	}

	// Si le bonus double balle est actif, 2 balles apparaissent au lieu d'un
	if(state->bonus == 10 || (state->bonus == 9 && state->lives == 5))
		numBall = 2;
	else
		numBall = 1;

	// Position initiale des balles balle
	for(i = 0; i < MAXBALL; i++) {
        InitBall(state, i);
        if (i < numBall) {
            // Les balles sont de type explode si le bonus explosif est actif
			if (state->bonus == 8 || (state->bonus == 7 && state->lives == 5))
				state->ball[i].type = BALL_EXPLODE;
			else
				state->ball[i].type = BALL_CLASSIC;
			// La balle est initialement collée a la paddle
			state->ball[i].stuck = PP_TRUE;
			state->ball[i].speed = BALLSPEED;
			// Sa vélocité est null
			state->ball[i].velocity = PP_MakeFloatPoint(0, 0);
		} else {
            state->ball[i].type = BALL_NONE;
		}
	}

	// Même si le bonus double balle est actif, ballCount est initialisé a 1
	// afin de garantir la présence de 2 balles en permanence
	state->ballCount = 1;

	// Position initiale de la paddle
	// Sa taille est plus grande si le bonus paddle géante est actif
	if(state->bonus == 3)
		state->paddle.surf = PP_MakeRectFromXYWH(1100/2 - 50, 0, MAXPADDLE,
												 state->textures[6].height);
	else
		state->paddle.surf = PP_MakeRectFromXYWH(1100/2 - 50, 0, BASEPADDLE,
												 state->textures[6].height);
	// Et la paddle est collante si le bonus correspondant est actif
	if(state->bonus == 6 || (state->bonus == 5 && state->lives == 5))
		state->paddle.sticky = PP_TRUE;
	else
		state->paddle.sticky = PP_FALSE;
}

// Appelée quand la souris est vérrouillée
void MouseLocked(void* data, int32_t result) {
	if(result == PP_OK) {
		// Passage du programe a l'état en jeu
		Jeu* state = (Jeu*) data;
		state->newState = STATE_INGAME;
	} else {
		PostMessage("PointerLock error %d", result);
	}
}

// Detection des collisions
int collides(Ball a, struct PP_Rect b) {
	return a.pos.x - a.radius < b.point.x + b.size.width &&
		a.pos.x + a.radius > b.point.x &&
		a.pos.y - a.radius < b.point.y + b.size.height &&
		a.pos.y + a.radius > b.point.y;
}

// Collision avec les murs
void WallsCollision(PSContext2D_t* ctx, Jeu* state, int i) {
	// Bord gauche
	if (state->ball[i].pos.x - state->ball[i].radius <= 0) {
		state->ball[i].pos.x = state->ball[i].radius + 1;
		state->ball[i].velocity.x = fabs(state->ball[i].velocity.x);
	}

	// Bord droit
	if (state->ball[i].pos.x + state->ball[i].radius >= ctx->width) {
		state->ball[i].pos.x = ctx->width - state->ball[i].radius - 1;
		state->ball[i].velocity.x = -fabs(state->ball[i].velocity.x);
	}

	// Bord supérieur
	if (state->ball[i].pos.y - state->ball[i].radius <= 0) {
		state->ball[i].pos.y = state->ball[i].radius + 1;
		state->ball[i].velocity.y = fabs(state->ball[i].velocity.y);
	}
}

// Collision avec la paddle
void PaddleCollision(PSContext2D_t* ctx, Jeu* state, int i) {
	// Nouvel angle de la balle
	float angle = ((90 * (
		(state->ball[i].pos.x - state->paddle.surf.point.x) /
		state->paddle.surf.size.width)) / 2) - (45 / 2),
	// Nouvelle vélocité a partir de cet angle
		x = sin(angle / 180 * PI) * state->ball[i].speed,
		y = cos(angle / 180 * PI) * state->ball[i].speed;
	state->ball[i].velocity.x = x;
	// La vélocité sur y est inversé car l'origine se trouve en haut a gauche
	// et non en bas
	state->ball[i].velocity.y = -y;
}

// Collision avec une brique
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
			// Spawn d'un drop
			if (rand() < (RAND_MAX / 3))
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

// Apparition d'un drop
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

// Initialisation d'un balle
void InitBall(Jeu* state, int i) {
    // Si le bonus balle gelée est actif
	if (state->bonus == 4)
		state->ball[i].speed = MINSPEED;
	else
		state->ball[i].speed = BALLSPEED;
	state->ball[i].velocity = PP_MakeFloatPoint(0, 0);
    state->ball[i].pos = PP_MakeFloatPoint(1100/2, 700 - 20);
    state->ball[i].radius = BALLRADIUS;
	state->ball[i].stuck = PP_FALSE;
}

// Ajoute une balle dans le jeu
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
