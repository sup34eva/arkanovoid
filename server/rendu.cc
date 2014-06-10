// Copyright 2014 Huns de Troyes
#include "include/rendu.h"

void TitleDraw(PSContext2D_t* ctx) {
	memset(ctx->data, 0x00, ctx->stride * ctx->height);
}

// Dessine une frame
void GameDraw(PSContext2D_t* ctx, Jeu state) {
	PSContext2DGetBuffer(ctx);

	if (NULL == ctx->data)
		return;

	memset(ctx->data, 0x00, ctx->stride * ctx->height);
	/*DrawTexture(ctx, PP_MakePoint(0, 0), &texture_layer4);
	DrawTexture(ctx, PP_MakePoint(0, 0), &texture_layer3);
	DrawTexture(ctx, PP_MakePoint(0, 0), &texture_layer2);
	DrawTexture(ctx, PP_MakePoint(0, 0), &texture_layer1);*/

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

			if(a > 0) {
				r = (r * (a / 255)) + (
						((ctx->data[ctx->width * py + px] & 0xff0000) >> 16)
					 * (1 - (a / 255)));
				g = (g * (a / 255)) + (
						((ctx->data[ctx->width * py + px] & 0xff00) >> 8)
					 * (1 - (a / 255)));
				b = (b * (a / 255)) + (
						(ctx->data[ctx->width * py + px] & 0xff)
					 * (1 - (a / 255)));
				ctx->data[ctx->width * py + px] = RGBA(r, g, b, a);
			}
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
