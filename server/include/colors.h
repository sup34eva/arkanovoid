// Copyright 2014 Huns de Troyes
#ifndef SERVER_INCLUDE_COLORS_H_
#define SERVER_INCLUDE_COLORS_H_

#define RGBA(r, g, b, a) (a << 24) | (r << 16) | (g << 8) | b
#define RGB(r, g, b) (255 << 24) | (r << 16) | (g << 8) | b
#define getRed(c) ((c & 0xff0000) >> 16)
#define getGreen(c) ((c & 0xff00) >> 8)
#define getBlue(c) (c & 0xff)
#define getAlpha(c) ((c & 0xff000000) >> 24)

#define COLOR_INVIS	RGBA(0, 0, 0, 0)
#define COLOR_BLACK	RGB(0, 0, 0)
#define COLOR_LGREY	RGB(211, 211, 211)
#define COLOR_GREY	RGB(128, 128, 128)
#define COLOR_RED 	RGB(255, 0, 0)
#define COLOR_YELLOW RGB(255, 255, 0)
#define COLOR_GREEN	RGB(0, 255, 0)
#define COLOR_BLUE	RGB(0, 0, 255)
#define COLOR_WHITE	RGB(255, 255, 255)

#endif  // SERVER_INCLUDE_COLORS_H_
