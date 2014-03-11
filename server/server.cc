#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"

class ServerInstance : public pp::Instance {
	public:
		explicit ServerInstance(PP_Instance instance) : pp::Instance(instance) {}
		virtual ~ServerInstance() {}

		virtual void HandleMessage(const pp::Var& var_message) {
			PostMessage(var_message);
		}
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
}
