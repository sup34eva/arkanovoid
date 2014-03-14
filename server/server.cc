#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "jeu.h"

class ServerInstance : public pp::Instance {
	public:
		explicit ServerInstance(PP_Instance instance) : pp::Instance(instance) {
			// TODO: Contructor
		}
		virtual ~ServerInstance() {}

		virtual void HandleMessage(const pp::Var& var_message) {
			if(var_message.is_number())
				loop(var_message.AsDouble());
			if(var_message.is_string())
				message(var_message.AsString());
		}

		virtual bool Init ( uint32_t argc, const char * argn[], const char * argv[]) {
			srand(time(NULL));

			for(int i = 0; i < 10; i++)
					state.niveau |= 1 << rand() % sizeof(int);

			state.balle.Velocity.X = BALLSPEED;
			state.balle.Velocity.Y = BALLSPEED;
			state.balle.Position.X = 0;
			state.balle.Position.Y = 0;
			state.paddle.Position = 0;
			state.paddle.Taille = 5;

			return true;
		}

	protected:
		struct Game state;

		void loop (double dt) {
			calc(dt);
			PostMessage(getState());
		}

		void calc (double deltaTime) {
		        state.balle.Position.X += state.balle.Velocity.X * deltaTime;
		        state.balle.Position.Y += state.balle.Velocity.Y * deltaTime;
		        // TODO: A remplacer par une position calculée par rapport aux events clavier/souris
		        state.paddle.Position = state.balle.Position.Y - state.paddle.Taille/2;
		        state.paddle.Position = fmin(fmax(state.paddle.Position, 0), SCREENSIZE - state.paddle.Taille);

		        // Gestion des collisions basique
		        if(state.balle.Position.X >= SCREENSIZE)
		                state.balle.Velocity.X = -abs(state.balle.Velocity.X);
		        if(state.balle.Position.X <= 0)
		                state.balle.Velocity.X = abs(state.balle.Velocity.X);
		        if(state.balle.Position.Y >= SCREENSIZE)
		                state.balle.Velocity.Y = -abs(state.balle.Velocity.Y);
		        if(state.balle.Position.Y <= 0)
		                state.balle.Velocity.Y = abs(state.balle.Velocity.Y);
		}

		void message (std::string message) {
			// TODO: Gestion des messages
		}

		std::string getState() {
			char result[(3*4) + (4) + (8 * 8)];
			sprintf(result, "%f:%f:%f:%d:%d", state.balle.Position.X, state.balle.Position.Y, state.paddle.Position, state.paddle.Taille, state.niveau);
			return std::string(result);
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

