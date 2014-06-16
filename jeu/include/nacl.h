// Copyright 2014 Huns De Troyes

#ifndef JEU_INCLUDE_NACL_H_
#define JEU_INCLUDE_NACL_H_

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
#include "ppapi/c/ppp_mouse_lock.h"
#include "ppapi/c/ppb_var_array.h"

#ifdef SEL_LDR
#define server_main main
#endif

#endif  // JEU_INCLUDE_NACL_H_
