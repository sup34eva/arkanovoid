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
			if(var_message.is_dictionary())
				message(var_message);
		}

		virtual bool Init ( uint32_t argc, const char * argn[], const char * argv[]) {
			srand(time(NULL));

			pp::VarArray briques;
			for(int x = 0; x < BRICKH; x++) {
				pp::VarArray array;
				for(int y = 0; y < BRICKW; y++)
					array.Set(y, pp::Var(rand() % 3));
				briques.Set(x, array);
			}

			state.Set(pp::Var("x"), pp::Var(0));
			state.Set(pp::Var("y"), pp::Var(0));
			state.Set(pp::Var("pos"), pp::Var(0));
			state.Set(pp::Var("size"), pp::Var(5));
			state.Set(pp::Var("brick"), briques);

			velocity.X = BALLSPEED;
			velocity.Y = BALLSPEED;

			return true;
		}

	protected:
		pp::VarDictionary state;
		struct Vector velocity;
		int inputs[222];

		void loop (double dt) {
			calc(dt);
			PostMessage(state);
		}

		void calc (double deltaTime) {
		        // Déplacement de la balle
				state.Set(pp::Var("x"), pp::Var(state.Get(pp::Var("x")).AsDouble() + (velocity.X * deltaTime)));
		        state.Set(pp::Var("y"), pp::Var(state.Get(pp::Var("y")).AsDouble() + (velocity.Y * deltaTime)));

		        // Gestion de la paddle
				if(inputs[37] == 1)
					state.Set(pp::Var("pos"), pp::Var(state.Get(pp::Var("pos")).AsDouble() - (BALLSPEED * deltaTime)));
				if(inputs[39] == 1)
					state.Set(pp::Var("pos"), pp::Var(state.Get(pp::Var("pos")).AsDouble() + (BALLSPEED * deltaTime)));
				state.Set(pp::Var("pos"), pp::Var(fmin(fmax(state.Get(pp::Var("pos")).AsDouble(), 0), SCREENSIZE - state.Get(pp::Var("size")).AsDouble())));

		        // Gestion des collisions basique
		        if(state.Get(pp::Var("x")).AsDouble() >= SCREENSIZE)
		                velocity.X = -abs(velocity.X);
		        if(state.Get(pp::Var("x")).AsDouble() <= 0)
		                velocity.X = abs(velocity.X);
		        if(state.Get(pp::Var("y")).AsDouble() >= SCREENSIZE)
		                velocity.Y = -abs(velocity.Y);
		        if(state.Get(pp::Var("y")).AsDouble() <= 0)
		                velocity.Y = abs(velocity.Y);
		}

		void message (pp::Var message) {
			pp::VarDictionary msg(message);
			// Deplacement de la souris
			if(msg.Get(pp::Var("name")).AsString() == "m")
				state.Set(pp::Var("pos"), pp::Var(state.Get(pp::Var("pos")).AsDouble() + msg.Get(pp::Var("data")).AsDouble()));
			// Touche enfoncée
			if(msg.Get(pp::Var("name")).AsString() == "+")
				inputs[msg.Get(pp::Var("data")).AsInt()] = 1;
			// Touche relachée
			if(msg.Get(pp::Var("name")).AsString() == "-")
				inputs[msg.Get(pp::Var("data")).AsInt()] = 0;
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

