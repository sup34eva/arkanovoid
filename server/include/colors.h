// Copyright 2014 Huns de Troyes
#ifndef SERVER_INCLUDE_COLORS_H_
#define SERVER_INCLUDE_COLORS_H_

#define COLOR_INVIS	0x00000000
#define COLOR_BLACK	0xff000000
#define COLOR_LGREY	0xffd3d3d3
#define COLOR_GREY	0xff808080
#define COLOR_RED 	0xffff0000
#define COLOR_YELLOW	0xffffff00
#define COLOR_GREEN	0xff00ff00
#define COLOR_BLUE	0xff0000ff
#define COLOR_WHITE	0xffffffff

#define RGB(r, g, b) (a << 24) | (r << 16) | (g << 8) | b
#define RGBA(r, g, b, a) (255 << 24) | (r << 16) | (g << 8) | b
#define getRed(c) ((c & 0xff0000) >> 16)
#define getGreen(c) ((c & 0xff00) >> 8)
#define getBlue(c) (c & 0xff)
#define getAlpha(c) ((c & 0xff000000) >> 24)

#endif  // SERVER_INCLUDE_COLORS_H_
