// Copyright 2014 Huns De Troyes

#ifndef SERVER_INCLUDE_MAIN_H_
#define SERVER_INCLUDE_MAIN_H_

#include <stdio.h>
#include <math.h>
#include <stdarg.h>

#include "nacl.h"

#ifdef SEL_LDR
#define server_main main
#endif

#include "const.h"
#include "colors.h"
#include "types.h"

#include "calc.h"
#include "rendu.h"

char* VprintfToNewString(const char* format, va_list args);
void PostMessage(const char *format, ...);

#endif  // SERVER_INCLUDE_MAIN_H_
