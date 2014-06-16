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
	state->textures[2] = LoadTexture("/img/textures/share.tex");
	state->textures[3] = LoadTexture("/img/textures/loading.tex");

	state->textures[9] = LoadTexture("/img/textures/paddle_plus.tex");
	state->textures[13] = LoadTexture("/img/textures/speed_less.tex");
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
	state->textures[15] = LoadTexture("/img/textures/skull.tex");
	state->textures[16] = LoadTexture("/img/textures/speed_plus.tex");

	state->textures[17] = LoadTexture("/img/textures/glue_left.tex");
	state->textures[18] = LoadTexture("/img/textures/glue_middle.tex");
	state->textures[19] = LoadTexture("/img/textures/glue_right.tex");

	state->textures[20] = LoadTexture("/img/textures/bomb.tex");

	state->textures[21] = LoadTexture("/img/textures/font.tex");
	state->textures[22] = LoadTexture("/img/textures/score.tex");
}

void DrawLoadingScreen(PSContext2D_t* ctx, Jeu* state) {
	PSContext2DGetBuffer(ctx);
	DrawRect(ctx,
			 PP_MakeRectFromXYWH(0, 0, ctx->width, ctx->height),
			 RGBA(0, 0, 0, 128));
	DrawTexture(ctx,
				PP_MakePoint(ctx->width / 2, ctx->height / 2),
				state->textures[3]);
	PSContext2DSwapBuffer(ctx);
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
		j += ((origin.x + tex.width) - px) * tex.channels;
	}
}

// NOTE: La largeur de caractère est hardcodée a 26px
void DrawText(PSContext2D_t* ctx,
			  struct PP_Point origin,
			  const char* text,
			  const Texture font) {
	unsigned int i, j, px, py, length = strlen(text);
	for(i = 0; i < length; i++) {
		char ch = text[i];
		j = (ch - '0') * (font.channels * 26);
		for(py = origin.y; py < fmin(ctx->height, origin.y + font.height); py++) {
			for(px = origin.x; px < fmin(ctx->width, origin.x + 26); px++) {
				unsigned int r, g, b, a = 255;

				// Récupération des différentes chaines de la texture
				r = font.pixel_data[j++];
				g = font.pixel_data[j++];
				b = font.pixel_data[j++];

				if(font.channels == 4) {
					a = font.pixel_data[j++];
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
			j += ((origin.x + font.width) - px) * font.channels;
		}
		origin.x += 26;
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
