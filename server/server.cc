#include "jeu.h"

class ServerInstance : public pp::Instance {
	public:
		explicit ServerInstance(PP_Instance instance) : pp::Instance(instance) {
			remaining = 0;
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

			#ifdef BRICKS
			// Génération de niveau random
			for(int x = 0; x < BRICKH; x++) {
				pp::VarArray array;
				for(int y = 0, val; y < BRICKW; y++) {
					val = rand() % 5;
					array.Set(y, val);
					if(val > 0 && val < 4)
						remaining++;
				}
				briques.Set(x, array);
			}
			#endif

			x = 0;
			y = 0;
			pos = 0;
			size = 5;
			prevX = 0;
			prevY = 0;

			velocity.X = BALLSPEED;
			velocity.Y = BALLSPEED;

			return true;
		}

	protected:
		pp::VarDictionary state;
		double x, y, pos, size;
		pp::VarArray briques, row;
		struct Vector velocity;
		int inputs[222], prevX, prevY, brickX, brickY, brick, remaining, breakable, exists;

		void loop (double dt) {
			Calc(dt);

			state.Set(pp::Var("x"), x);
			state.Set(pp::Var("y"), y);
			state.Set(pp::Var("pos"), pos);
			state.Set(pp::Var("size"), size);
			#ifdef BRICKS
			briques.Set(brickY, row);
			state.Set(pp::Var("brick"), briques);
			state.Set(pp::Var("win"), remaining == 0);
			#endif

			PostMessage(state);

			PostCalc();
		}

		void Calc (double deltaTime) {
			// Déplacement de la balle
			x += velocity.X * deltaTime;
			y += velocity.Y * deltaTime;

			// Gestion de la paddle
			if(inputs[37] == 1)
				pos -= BALLSPEED * deltaTime;
			if(inputs[39] == 1)
				pos += BALLSPEED * deltaTime;
			pos = fmin(fmax(pos, 0), SCREENSIZE - size);

			// Gestion des collisions basique
			#ifdef BRICKS
			brickX = x / (100/BRICKW);
			brickY = y / (100/BRICKH);
			if(brickY != prevY)
				row = briques.Get(brickY);
			brick = row.Get(brickX).AsInt();
			exists = brick > 0;
			breakable = brick < 4;

			if(exists && breakable) {
				row.Set(brickX, brick - 1);
				if(brick == 1)
					remaining--;
			}
			#endif
		}

		void PostCalc() {
			if(x >= SCREENSIZE || (exists && prevX < brickX))
				velocity.X = -abs(velocity.X);
			if(x <= 0 || (exists && prevX > brickX))
				velocity.X = abs(velocity.X);
			if(exists && prevY < brickY)
				velocity.Y = -abs(velocity.Y);
			if(y <= 0 || (exists && prevY > brickY))
				velocity.Y = abs(velocity.Y);

			if(y >= SCREENSIZE - 1 && y <= SCREENSIZE && x >= pos && x <= pos + size) {
				int angle = 45 + (((x - pos)/size) * 90);
				velocity.Y = sin(angle) * BALLSPEED;
				velocity.X = cos(angle) * BALLSPEED;
			}

			prevX = brickX;
			prevY = brickY;
		}

		void message (pp::Var message) {
			pp::VarDictionary msg(message);
			// Deplacement de la souris
			if(msg.Get(pp::Var("name")).AsString() == "m")
				pos += msg.Get(pp::Var("data")).AsDouble();
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

