// Copyright 2014 Huns de Troyes
#include "include/jeu.h"

// Fonction main du programme
int server_main(int argc, char* argv[]) {
	PSContext2D_t* context = PSContext2DAllocate(PP_IMAGEDATAFORMAT_BGRA_PREMUL);
	Jeu gamestate;

	// Charge les interfaces
	g_pInputEvent =
		(PPB_InputEvent*) PSGetInterface(PPB_INPUT_EVENT_INTERFACE);
	g_pKeyboardInput = (PPB_KeyboardInputEvent*)
		PSGetInterface(PPB_KEYBOARD_INPUT_EVENT_INTERFACE);
	g_pMouseInput =
		(PPB_MouseInputEvent*) PSGetInterface(PPB_MOUSE_INPUT_EVENT_INTERFACE);

	Init(context, &gamestate);

	while (gamestate.brickCount > 0) {
			PSEvent* event;

			// TODO: Activer la reception d'evenements
			while ((event = PSEventTryAcquire()) != NULL) {
				PSContext2DHandleEvent(context, event);
				HandleEvent(event, &gamestate, context);
				PSEventRelease(event);
			}

			if (context->bound) {
				Calc(context, &gamestate);
				Draw(context, gamestate);
			}
	}

	PSContext2DFree(context);
	return 0;
}

void PostMessage(const char *format, ...) {
	va_list ap;
	char buffer[100];
	va_start(ap, format);
	vsnprintf(buffer, sizeof buffer, format, ap);
	va_end(ap);
	PSInterfaceMessaging()->PostMessage(PSGetInstanceId(),
										PSInterfaceVar()->VarFromUtf8(buffer,
																	  sizeof buffer));
}

void PostNumber(double num) {
	PSInterfaceMessaging()->PostMessage(PSGetInstanceId(), PP_MakeDouble(num));
}

// Determine la distance entre 2 points
float Dist(struct PP_FloatPoint from, struct PP_FloatPoint to) {
	return sqrt(pow(from.x - to.x, 2) + pow(from.y - to.y, 2));
}

/*Texture makeTexture(uint8_t* data, PP_Size size, int format) {
	int i;
	Texture tex;
	tex.size = size;
	for(i = 0; i < size.height * size.width; i += 4)
		tex.data[i] = RGBA(data[i], data[i+1], data[i+2], data[i+3]);
	return tex;
}*/

// Initialise l'état de la partie
void Init(PSContext2D_t* ctx, Jeu* state) {
	PSInstance::GetInstance()->SetEnabledEvents(PSE_ALL);
	PSInstance::GetInstance()->RequestInputEvents(PP_INPUTEVENT_CLASS_MOUSE |
												  PP_INPUTEVENT_CLASS_KEYBOARD);

	/*PP_CompletionCallback cb;
	cb.flags = 0;
	cb.user_data = ctx;
	cb.func = MouseLockCallback;
	s_MouseLock = static_cast<const PPB_MouseLock*>(PSGetInterface(PPB_MOUSELOCK_INTERFACE));
	s_MouseLock->LockMouse(PSGetInstanceId(), cb);*/

	srand(time(NULL));

	int i, j;
	state->brickCount = 0;
	for(i = 0; i < BRICKW; i++) {
		for(j = 0; j < BRICKH; j++) {
			if(j < BRICKH - 2) {
				switch(rand() % 5) {
						case 0 :
							state->bricks[i][j] = BRICK_NONE;
							break;
						case 1 :
							state->bricks[i][j] = BRICK_ONETOUCH;
							state->brickCount++;
							break;
						case 2 :
						case 3 :
							state->bricks[i][j] = BRICK_THREETOUCH;
							state->brickCount++;
							break;
						case 4 :
							state->bricks[i][j] = BRICK_UBER;
							state->brickCount++;
							break;
				}
			} else {
				state->bricks[i][j] = BRICK_NONE;
			}
		}
	}

	for(i = 0; i < MAXDROP; i++) {
		state->drops[i].pos = PP_MakeFloatPoint(0, 0);
		state->drops[i].type = DROP_NONE;
	}

	state->ball.pos = PP_MakeFloatPoint(250, 450);
	float x = sin((PI / 180) * STARTANGLE) * BALLSPEED,
		y = cos((PI / 180) * STARTANGLE) * BALLSPEED;
	state->ball.velocity = PP_MakeFloatPoint(x, y);
	state->ball.radius = 5;

	state->paddle = PP_MakeRectFromXYWH(0, 0, 100, 20);
}

float clamp(float val, float low, float high) {
	return fmin(fmax(val, low), high);
}

// Gère les évenements
void HandleEvent(PSEvent* event, Jeu* state, PSContext2D_t* ctx) {
	if (event->type == PSE_INSTANCE_HANDLEINPUT) {
		switch (g_pInputEvent->GetType(event->as_resource)) {
			case PP_INPUTEVENT_TYPE_KEYDOWN: {
				uint32_t key_code = g_pKeyboardInput->GetKeyCode(event->as_resource);
				PostMessage("%d", key_code);
				switch(key_code) {
						case 39 :
							state->paddle.point.x += ctx->width / 50;
							break;
						case 37 :
							state->paddle.point.x -= ctx->width / 50;
							break;
				}
				break;
			}
			case PP_INPUTEVENT_TYPE_MOUSEMOVE: {
				struct PP_Point movement = g_pMouseInput->GetMovement(event->as_resource);
				state->paddle.point.x += movement.x;
				break;
			}
			default:
				break;
		}
	}
}

// Determine si un point est contenu dans un rectangle
int Contains(struct PP_Rect rect, PP_FloatPoint point) {
  return (point.x >= rect.point.x) &&
	  (point.x < (rect.point.x + rect.size.width)) &&
	  (point.y >= rect.point.y) &&
	  (point.x < (rect.point.y + rect.size.height));
}

// Met a jour l'état du jeu
void Calc(PSContext2D_t* ctx, Jeu* state) {
	int h = ctx->height / BRICKH, w = ctx->width / BRICKW,
		lastX = clamp(state->ball.pos.x, 1, ctx->width),
		lastY = clamp(state->ball.pos.y, 1, ctx->height);

	state->paddle.point.x = clamp(state->paddle.point.x, 0,
								  ctx->width - state->paddle.size.width);

	state->ball.pos.x += state->ball.velocity.x;
	state->ball.pos.y += state->ball.velocity.y;

	if(state->ball.pos.x - state->ball.radius <= 0) {
		state->ball.pos.x = state->ball.radius + 1;
		state->ball.velocity.x = abs(state->ball.velocity.x);
	}

	if(state->ball.pos.x + state->ball.radius >= ctx->width) {
		state->ball.pos.x = ctx->width - state->ball.radius - 1;
		state->ball.velocity.x = -abs(state->ball.velocity.x);
	}

	if(state->ball.pos.y - state->ball.radius <= 0) {
		state->ball.pos.y = state->ball.radius + 1;
		state->ball.velocity.y = abs(state->ball.velocity.y);
	}

	if(state->ball.pos.y + state->ball.radius>
	   ctx->height - state->paddle.size.height &&
	   state->ball.pos.x >= state->paddle.point.x &&
	   state->ball.pos.x <= state->paddle.point.x + state->paddle.size.width) {
		state->ball.pos.y = ctx->height - (
			state->paddle.size.height + state->ball.radius + 1);
		float angle = ((90 * (
			(state->ball.pos.x - state->paddle.point.x) /
			state->paddle.size.width)) / 2) - (45 / 2),
			x = sin((PI / 180) * angle) * BALLSPEED,
			y = cos((PI / 180) * angle) * BALLSPEED;
		state->ball.velocity.x = x;
		state->ball.velocity.y = -y;
	}

	// TODO: Prendre en compte le rayon de la balle
	if(h != 0 && w != 0) {
		int brickX = state->ball.pos.x / w, brickY = state->ball.pos.y / h,
			lastBrickX = lastX / w, lastBrickY = lastY / h;

		if(state->bricks[brickX][brickY] != BRICK_NONE) {
			switch(state->bricks[brickX][brickY]) {
				case BRICK_ONETOUCH:
					state->bricks[brickX][brickY] = BRICK_NONE;
					state->brickCount--;
					break;
				case BRICK_TWOTOUCH:
					state->bricks[brickX][brickY] = BRICK_ONETOUCH;
					break;
				case BRICK_THREETOUCH:
					state->bricks[brickX][brickY] = BRICK_TWOTOUCH;
					break;
				default:
					break;
			}

			// TODO: Ajouter des drops

			if(((state->ball.pos.x + state->ball.radius) / w) > lastBrickX)
				state->ball.velocity.x = -abs(state->ball.velocity.x);

			if(((state->ball.pos.x - state->ball.radius) / w) < lastBrickX)
				state->ball.velocity.x = abs(state->ball.velocity.x);

			if(((state->ball.pos.y + state->ball.radius) / h) > lastBrickY)
				state->ball.velocity.y = -abs(state->ball.velocity.y);

			if(((state->ball.pos.y - state->ball.radius) / h) < lastBrickY)
				state->ball.velocity.y = abs(state->ball.velocity.y);
		}
	}
}

// Dessine un rectangle dans un contexte
void DrawRect(PSContext2D_t* ctx, struct PP_Rect rect, uint32_t color) {
	int32_t px, py, right, bottom;

	for(px = clamp(rect.point.x, 0, ctx->width),
		right = clamp(rect.point.x + rect.size.width, 0, ctx->width);
		px < right; px++) {
		for(py = clamp(rect.point.y, 0, ctx->height),
			bottom = clamp(rect.point.y + rect.size.height, 0, ctx->height);
			py < bottom; py++) {
			ctx->data[ctx->width * py + px] = color;
		}
	}
}

// Dessine une texture dans un contexte
void DrawTexture(PSContext2D_t* ctx,
				 struct PP_Point origin,
				 const Texture* tex) {
	int px, py, j = 0;
	for(py = origin.y; py < fmin(ctx->height, origin.y + tex->height); py++) {
		for(px = origin.x; px < fmin(ctx->width, origin.x + tex->width); px++) {
			uint8_t r, g, b, a = 255;

			r = tex->pixel_data[j++];
			g = tex->pixel_data[j++];
			b = tex->pixel_data[j++];

			if(tex->bytes_per_pixel == 4) {
				a = tex->pixel_data[j++];
			}

			if(a > 128)
				ctx->data[ctx->width * py + px] = RGBA(r, g, b, a);
		}
	}
}

// Dessine un cercle dans un contexte
void DrawCircle(PSContext2D_t* ctx,
				struct PP_FloatPoint center,
				int32_t radius, uint32_t color) {
	int32_t px, py,
	right = clamp(center.x + radius, 0, ctx->width),
	bottom = clamp(center.y + radius, 0, ctx->width);

	for(px = clamp(center.x - radius, 0, ctx->width); px < right; px++) {
		for(py = clamp(center.y - radius, 0, ctx->height); py < bottom; py++) {
			if(Dist(PP_MakeFloatPoint(px, py), center) < radius)
				ctx->data[ctx->width * py + px] = color;
		}
	}
}

// Dessine une frame
void Draw(PSContext2D_t* ctx, Jeu state) {
	PSContext2DGetBuffer(ctx);

	if (NULL == ctx->data)
		return;
	DrawTexture(ctx, PP_MakePoint(0, 0), &texture_layer1);

	int i, j;

	// Briques
	for(i = 0; i < BRICKW; i++)
		for(j = 0; j < BRICKH; j++) {
			PP_Point p = PP_MakePoint(i * (ctx->width / BRICKW),
									  j * (ctx->height / BRICKH));
			switch(state.bricks[i][j]) {
				case BRICK_ONETOUCH:
					DrawTexture(ctx, p, &texture_brick1);
					break;
				case BRICK_TWOTOUCH:
					DrawTexture(ctx, p, &texture_brick2);
					break;
				case BRICK_THREETOUCH:
					DrawTexture(ctx, p, &texture_brick3);
					break;
				case BRICK_UBER:
					DrawTexture(ctx, p, &texture_brick4);
					break;
				default:
					break;
			}
		}

	// Drops
	for(i = 0; i < MAXDROP; i++) {
		if(state.drops[i].type != DROP_NONE)
			DrawCircle(ctx, state.drops[i].pos, 10, COLOR_GREEN);
	}

	// Paddle
	state.paddle.point.y = ctx->height - state.paddle.size.height;
	DrawTexture(ctx, state.paddle.point, &texture_paddle);

	// Balle
	DrawTexture(ctx,
				PP_MakePoint(state.ball.pos.x - state.ball.radius,
							 state.ball.pos.y - state.ball.radius),
				&texture_ball);

	PSContext2DSwapBuffer(ctx);
}

#ifndef SEL_LDR
PPAPI_SIMPLE_REGISTER_MAIN(server_main)
#endif
