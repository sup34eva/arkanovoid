// Copyright 2014 Huns de Troyes
#include "include/rendu.h"

Texture LoadTexture(const char* url) {
	struct {
		int  	 width;
		int  	 height;
		int  	 channels;
	} fmt;
	uint8_t* buffer;
	size_t len;

	mount("img", "/img", "httpfs", 0, "");

	FILE* fp = fopen(url, "rb");
	fread(&fmt, sizeof(fmt), 1, fp);

	len = fmt.width * fmt.height * fmt.channels + 1;
	buffer = new uint8_t[len];
	fread(buffer, 1, len, fp);
	fclose(fp);

	Texture tex = {fmt.width, fmt.height, fmt.channels, buffer};

	return tex;
}

void LoadTextures(Jeu* state) {
	state->textures[0] = LoadTexture("/img/ball.tex");
	state->textures[1] = LoadTexture("/img/brick1.tex");
	state->textures[2] = LoadTexture("/img/brick2.tex");
	state->textures[3] = LoadTexture("/img/brick3.tex");
	state->textures[4] = LoadTexture("/img/brick4.tex");
	state->textures[5] = LoadTexture("/img/paddle.tex");
}

void TitleDraw(PSContext2D_t* ctx, uint32_t color) {
	memset(ctx->data, color, ctx->stride * ctx->height);
}

// Dessine une frame
void GameDraw(PSContext2D_t* ctx, Jeu* state) {
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
			switch(state->bricks[i][j]) {
				case BRICK_ONETOUCH:
					DrawTexture(ctx, p, state->textures[1]);
					break;
				case BRICK_TWOTOUCH:
					DrawTexture(ctx, p, state->textures[2]);
					break;
				case BRICK_THREETOUCH:
					DrawTexture(ctx, p, state->textures[3]);
					break;
				case BRICK_UBER:
					DrawTexture(ctx, p, state->textures[4]);
					break;
				default:
					break;
			}
		}

	// Drops
	for(i = 0; i < MAXDROP; i++) {
		if(state->drops[i].type != DROP_NONE)
			DrawCircle(ctx, state->drops[i].pos, 10, COLOR_GREEN);
	}

	// Paddle
	state->paddle.point.y = ctx->height - state->paddle.size.height;
	DrawTexture(ctx, state->paddle.point, state->textures[5]);

	// Balle
	DrawTexture(ctx,
				PP_MakePoint(state->ball.pos.x - state->ball.radius,
							 state->ball.pos.y - state->ball.radius),
				state->textures[0]);

	PSContext2DSwapBuffer(ctx);
}

// Dessine une texture dans un contexte
void DrawTexture(PSContext2D_t* ctx,
				 struct PP_Point origin,
				 const Texture tex) {
	int px, py, j = 0;
	for(py = origin.y; py < fmin(ctx->height, origin.y + tex.height); py++) {
		for(px = origin.x; px < fmin(ctx->width, origin.x + tex.width); px++) {
			uint8_t r, g, b, a = 255;

			r = tex.pixel_data[j++];
			g = tex.pixel_data[j++];
			b = tex.pixel_data[j++];

			if(tex.channels == 4) {
				a = tex.pixel_data[j++];
			}

			if(a > 0) {
				r = (r * (a / 255)) + (
						getRed(ctx->data[ctx->width * py + px])
					 * (1 - (a / 255)));
				g = (g * (a / 255)) + (
						getGreen(ctx->data[ctx->width * py + px])
					 * (1 - (a / 255)));
				b = (b * (a / 255)) + (
						getBlue(ctx->data[ctx->width * py + px])
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
