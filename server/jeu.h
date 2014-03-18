#ifndef MAIN_H
#define MAIN_H

#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/var_dictionary.h"
#include "ppapi/cpp/message_loop.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/utility/completion_callback_factory.h"
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define SCREENSIZE 100
#define BALLSPEED 40
#define BRICKH 10
#define BRICKW 8

struct Vector {
    double X;
    double Y;
};

class ServerInstance : public pp::Instance {
	public:
		explicit ServerInstance(PP_Instance instance);
		virtual ~ServerInstance();
		virtual void HandleMessage(const pp::Var& var_message);
		virtual bool Init ( uint32_t argc, const char * argn[], const char * argv[]);
		#ifdef MSGLOOP
		void Loop (int32_t result, clock_t lt);
		#else
		void Loop (double lt);
		#endif

	protected:
		pp::VarDictionary state;
		double x, y, pos, size;
		pp::VarArray briques, row;
		struct Vector velocity;
		int inputs[222], prevX, prevY, brickX, brickY, brick, remaining, breakable, exists;
		pp::MessageLoop msgLoop;
		pp::CompletionCallbackFactory<ServerInstance> factory_;

		void Calc (double deltaTime);
		void PostCalc();
		void message (pp::Var message);
};

class ServerModule : public pp::Module {
	public:
		ServerModule() : pp::Module() {}
		virtual ~ServerModule() {}

		virtual pp::Instance* CreateInstance(PP_Instance instance) {
			return new ServerInstance(instance);
		}
};

namespace pp {
	Module* CreateModule() {
		return new ServerModule();
	}
};

#endif

