// Copyright 2014 Huns De Troyes

#ifndef SERVER_INCLUDE_JEU_H_
#define SERVER_INCLUDE_JEU_H_

#include <stdio.h>
#include <math.h>
#include <stdarg.h>

// Charge les images
#include "../textures/layer1.h"
#include "../textures/paddle.h"
#include "../textures/ball.h"
#include "../textures/brick1.h"
#include "../textures/brick2.h"
#include "../textures/brick3.h"
#include "../textures/brick4.h"

// Charge l'API simple
#include "ppapi_simple/ps.h"
#include "ppapi_simple/ps_context_2d.h"
#include "ppapi_simple/ps_event.h"
#include "ppapi_simple/ps_interface.h"
#include "ppapi_simple/ps_main.h"
#include "ppapi_simple/ps_instance.h"

#ifdef WIN32
#undef PostMessage
#endif

#include "ppapi/c/ppb_input_event.h"
#include "ppapi/c/ppb_mouse_lock.h"

#ifdef SEL_LDR
#define server_main main
#endif

#include "const.h"
#include "colors.h"
#include "types.h"

PPB_InputEvent* g_pInputEvent;
PPB_KeyboardInputEvent* g_pKeyboardInput;
PPB_MouseInputEvent* g_pMouseInput;

void Init(PSContext2D_t*, Jeu* state);
void HandleEvent(PSEvent* event, Jeu* state, PSContext2D_t* ctx);
void Calc(PSContext2D_t* ctx, Jeu* state);
void Draw(PSContext2D_t* ctx, Jeu state);

static const PPB_MouseLock* s_MouseLock;
const PPB_MouseLock* PSInterfaceMouseLock() { return s_MouseLock; }

#endif  // SERVER_INCLUDE_JEU_H_
