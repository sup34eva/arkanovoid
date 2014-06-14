// Copyright 2014 Huns de Troyes
#include "include/rendu.h"

// Renvoie une texture chargée depuis une url données
Texture LoadTexture(const char* url) {
	// Structure stockant les metadonnées de l'image
	struct {
		int  	 width;
		int  	 height;
		int  	 channels;
	} meta;

	// Buffer contenant les données de l'image
	uint8_t* buffer;
	size_t len;

	// Montage du protocole http en tant que système de fichier
	mount("img", "/img", "httpfs", 0, "");

	// Ouverture de l'url
	FILE* fp = fopen(url, "rb");
	// Lecture des metadonnées
	fread(&meta, sizeof(meta), 1, fp);

	// Calcul de la taille du buffer
	len = meta.width * meta.height * meta.channels + 1;
	// Allocation du buffer
	buffer = (uint8_t*) malloc(sizeof(uint8_t) * len);
	// Lecture de l'image
	fread(buffer, 1, len, fp);
	// Fermeture du fichier
	fclose(fp);

	// Création de la structure texture
	Texture tex = {meta.width, meta.height, meta.channels, buffer};

	return tex;
}

void UnloadTexture(Texture tex) {
	free(tex.pixel_data);
}

void LoadTitleTextures(Jeu* state) {
	state->textures[8] = LoadTexture("/img/textures/lava.tex");
	state->textures[0] = LoadTexture("/img/textures/logo.tex");
	state->textures[1] = LoadTexture("/img/textures/button.tex");

	state->textures[9] = LoadTexture("/img/textures/paddle_plus.tex");
	state->textures[13] = LoadTexture("/img/textures/speed_less.tex");
	state->textures[15] = LoadTexture("/img/textures/skull.tex");
	state->textures[12] = LoadTexture("/img/textures/explode.tex");
	state->textures[10] = LoadTexture("/img/textures/sticky.tex");
	state->textures[11] = LoadTexture("/img/textures/clone.tex");
}

// Chargement des textures du jeu dans un tableau
void LoadGameTextures(Jeu* state) {
	state->textures[0] = LoadTexture("/img/textures/ball.tex");

	state->textures[1] = LoadTexture("/img/textures/brick1.tex");
	state->textures[2] = LoadTexture("/img/textures/brick2.tex");
	state->textures[3] = LoadTexture("/img/textures/brick3.tex");
	state->textures[4] = LoadTexture("/img/textures/brick4.tex");

	state->textures[5] = LoadTexture("/img/textures/paddle_left.tex");
	state->textures[6] = LoadTexture("/img/textures/paddle_middle.tex");
	state->textures[7] = LoadTexture("/img/textures/paddle_right.tex");

	state->textures[14] = LoadTexture("/img/textures/paddle_less.tex");
	state->textures[16] = LoadTexture("/img/textures/speed_plus.tex");

	state->textures[17] = LoadTexture("/img/textures/glue_left.tex");
	state->textures[18] = LoadTexture("/img/textures/glue_middle.tex");
	state->textures[19] = LoadTexture("/img/textures/glue_right.tex");

	state->textures[20] = LoadTexture("/img/textures/bomb.tex");
}

// Fonction appelée a chaque frame de l'écran titre pour dessiner l'image
void TitleDraw(PSContext2D_t* ctx, Jeu* state) {
	DrawTexture(ctx, PP_MakePoint(0, 0), state->textures[8]);

	// Affichage du logo
	PP_Point origin = PP_MakePoint((ctx->width / 2)
								   - (state->textures[0].width / 2), 0);
	DrawTexture(ctx, origin, state->textures[0]);

	// Affichage du bouton "nouvelle partie"
	origin = PP_MakePoint((ctx->width / 2) -
						  (state->textures[1].width / 2),
						  state->textures[0].height);
	DrawTexture(ctx, origin, state->textures[1]);

	int i;
	for(i = 0; i < 9; i++) {
		if(state->shop[i] == PP_TRUE) {
			Texture tex;
			PP_Point pos = PP_MakePoint(i * 20, ctx->height - 20);
			switch(i) {
				case 0:
					tex = state->textures[9];
					break;
				case 1:
					tex = state->textures[13];
					break;
				case 2:
					tex = state->textures[15];
					break;
				case 3:
					tex = state->textures[12];
					break;
				case 4:
					tex = state->textures[12];
					break;
				case 5:
					tex = state->textures[10];
					break;
				case 6:
					tex = state->textures[10];
					break;
				case 7:
					tex = state->textures[11];
					break;
				case 8:
				default:
					tex = state->textures[11];
					break;
			}
			DrawTexture(ctx, pos, tex);
		}
	}
}

void PauseDraw(PSContext2D_t* ctx, Jeu* state) {
	GameDraw(ctx, state);
	DrawRect(ctx, PP_MakeRectFromXYWH(0, 0, ctx->width, ctx->height),
			 RGBA(0, 0, 0, 128));
}

// Fonction appelée a chaque frame de jeu pour mettre a jour l'image
void GameDraw(PSContext2D_t* ctx, Jeu* state) {
	// Effacement de l'écran
	// memset(ctx->data, 0x00, ctx->width * ctx->height * 4);
	DrawTexture(ctx, PP_MakePoint(0, 0), state->textures[8]);

	int i, j;

	for(i = 0; i < state->lives; i++) {
		DrawTexture(ctx,
					PP_MakePoint(state->textures[0].width * i, 0),
					state->textures[0]);
	}

	// Affichage des briques
	for(i = 0; i < BRICKW; i++)
		for(j = 0; j < BRICKH; j++) {
			// Calcul de la position de la texture
			PP_Point p = PP_MakePoint(i * (ctx->width / BRICKW),
									  j * (ctx->height / BRICKH));
			// Choix de la texture
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

	// Rendu des drops
	for(i = 0; i < MAXDROP; i++) {
		switch(state->drops[i].type) {
            case DROP_PADDLE_PLUS:
                DrawTexture(ctx,
							PP_MakePoint(state->drops[i].pos.x - 15,
										 state->drops[i].pos.y - 15),
							state->textures[9]);
                break;
            case DROP_STICK:
                DrawTexture(ctx,
							PP_MakePoint(state->drops[i].pos.x - 15,
										 state->drops[i].pos.y - 15),
							state->textures[10]);
                break;
            case DROP_CLONE:
                DrawTexture(ctx,
							PP_MakePoint(state->drops[i].pos.x - 15,
										 state->drops[i].pos.y - 15),
							state->textures[11]);
                break;
            case DROP_EXPLODE:
                DrawTexture(ctx,
							PP_MakePoint(state->drops[i].pos.x - 15,
										 state->drops[i].pos.y - 15),
							state->textures[12]);
                break;
            case DROP_SPEED_LESS:
                DrawTexture(ctx,
							PP_MakePoint(state->drops[i].pos.x - 15,
										 state->drops[i].pos.y - 15),
							state->textures[13]);
                break;
            case DROP_PADDLE_LESS:
                DrawTexture(ctx,
							PP_MakePoint(state->drops[i].pos.x - 15,
										 state->drops[i].pos.y - 15),
							state->textures[14]);
                break;
            case DROP_LOSE:
                DrawTexture(ctx,
							PP_MakePoint(state->drops[i].pos.x - 15,
										 state->drops[i].pos.y - 15),
							state->textures[15]);
                break;
            case DROP_SPEED_PLUS:
                DrawTexture(ctx,
							PP_MakePoint(state->drops[i].pos.x - 15,
										 state->drops[i].pos.y - 15),
							state->textures[16]);
                break;
            default:
                break;
		}
	}

	// Rendu de la paddle
	state->paddle.surf.point.y = ctx->height - state->paddle.surf.size.height;
	DrawTexture(ctx, state->paddle.surf.point, state->textures[5]);
	for(i = 0; i < state->paddle.surf.size.width - 30; i++)
		DrawTexture(ctx,
					PP_MakePoint(state->paddle.surf.point.x + 15 + i,
								 state->paddle.surf.point.y),
					state->textures[6]);
	DrawTexture(ctx,
				PP_MakePoint(state->paddle.surf.point.x +
							 (state->paddle.surf.size.width - 15),
							 state->paddle.surf.point.y), state->textures[7]);

	if(state->paddle.sticky == PP_TRUE) {
		DrawTexture(ctx, state->paddle.surf.point, state->textures[17]);
		for(i = 0; i < state->paddle.surf.size.width - 30; i += 50)
			DrawTexture(ctx,
						PP_MakePoint(state->paddle.surf.point.x + 15 + i,
									 state->paddle.surf.point.y),
						state->textures[18]);
		DrawTexture(ctx,
				PP_MakePoint(state->paddle.surf.point.x +
							 (state->paddle.surf.size.width - 15),
							 state->paddle.surf.point.y), state->textures[19]);
	}

	// Rendu de la balle
	for(i = 0; i < MAXBALL; i++)
		if (state->ball[i].type == BALL_CLASSIC) {
			DrawTexture(ctx,
						PP_MakePoint(state->ball[i].pos.x - state->ball[i].radius,
									 state->ball[i].pos.y - state->ball[i].radius),
						state->textures[0]);
		} else if (state->ball[i].type == BALL_EXPLODE) {
			DrawTexture(ctx,
						PP_MakePoint(state->ball[i].pos.x - state->ball[i].radius,
									 state->ball[i].pos.y - state->ball[i].radius - 10),
						state->textures[20]);
		}
}
// Alpha blending
uint32_t blend(uint32_t base, uint32_t blend) {
	unsigned int r = getRed(blend),
	g = getGreen(blend),
	b = getBlue(blend),
	a = getAlpha(blend) + 1;
	return RGB(((a * r + (256 - a) * getRed(base)) >> 8),
			   ((a * g + (256 - a) * getGreen(base)) >> 8),
			   ((a * b + (256 - a) * getBlue(base)) >> 8));
}

// Dessine une texture dans un contexte
void DrawTexture(PSContext2D_t* ctx,
				 struct PP_Point origin,
				 const Texture tex) {
	unsigned int px, py, j = 0;
	// Itération sur tous le pixel devant être écris
	for(py = origin.y; py < fmin(ctx->height, origin.y + tex.height); py++) {
		for(px = origin.x; px < fmin(ctx->width, origin.x + tex.width); px++) {
			unsigned int r, g, b, a = 255;

			// Récupération des différentes chaines de la texture
			r = tex.pixel_data[j++];
			g = tex.pixel_data[j++];
			b = tex.pixel_data[j++];

			if(tex.channels == 4) {
				a = tex.pixel_data[j++];
			}

			if(a > 0) {
				// Ecriture de la couleur du pixel
				if(a < 255)
					ctx->data[ctx->width * py + px] = blend(ctx->data[ctx->width * py + px],
															RGBA(r, g, b, a));
				else
					ctx->data[ctx->width * py + px] = RGBA(r, g, b, a);
			}
		}
		j += ((origin.x + tex.width) - px) * 4;
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
				ctx->data[ctx->width * py + px] = blend(
				ctx->data[ctx->width * py + px], color);
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
			ctx->data[ctx->width * py + px] = blend(
				ctx->data[ctx->width * py + px], color);
		}
	}
}
