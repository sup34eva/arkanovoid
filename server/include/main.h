// Copyright 2014 Huns De Troyes

#ifndef SERVER_INCLUDE_MAIN_H_
#define SERVER_INCLUDE_MAIN_H_

#include <stdio.h>
#include <math.h>
#include <stdarg.h>

#include "textures.h"
#include "nacl.h"

#ifdef SEL_LDR
#define server_main main
#endif

#include "const.h"
#include "colors.h"
#include "types.h"

#include "calc.h"
#include "rendu.h"
void Init(PSContext2D_t*, Jeu* state);

static const PPB_MouseLock* s_MouseLock;
const PPB_MouseLock* PSInterfaceMouseLock() { return s_MouseLock; }

#endif  // SERVER_INCLUDE_MAIN_H_
