// Copyright 2014 Huns De Troyes

#ifndef JEU_INCLUDE_TOOLS_H_
#define JEU_INCLUDE_TOOLS_H_

#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "nacl.h"

float Dist(struct PP_FloatPoint from, struct PP_FloatPoint to);
float clamp(float val, float low, float high);
char* VprintfToNewString(const char* format, va_list args);
void PostMessage(const char *format, ...);

#endif  // JEU_INCLUDE_TOOLS_H_
