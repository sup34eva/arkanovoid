// Copyright 2013 <Huns de Troyes>

#include "jeu.h"

ServerInstance::ServerInstance(PP_Instance instance)
	: pp::Instance(instance), factory_(this) {
	remaining = 0;
	finished = 0;
	msgLoop = pp::MessageLoop::GetForMainThread();
}

ServerInstance::~ServerInstance() {}

void ServerInstance::HandleMessage(const pp::Var& var_message) {
	if(var_message.is_dictionary())
		message(var_message);
}

bool ServerInstance::Init(uint32_t argc,
						  const char * argn[],
						  const char * argv[]) {
	srand(time(NULL));

	// Génération de niveau random
	// TODO: Implementer la méthode de Monte-Carlo (#10)
	for(int x = 0; x < BRICKW; x++) {
		for(int y = 0, val; y < BRICKH; y++) {
			val = rand() % 5;
			if(val > 0) {
				setBrick(x, y, val);
				briques[x][y] = val;
				if(val < 4)
					remaining++;
			}
		}
	}

	x = 0;
	y = 0;
	pos = 0;
	size = 5;
	prevX = 0;
	prevY = 0;

	velocity.X = BALLSPEED;
	velocity.Y = BALLSPEED;

	Loop(PP_OK, clock());

	return true;
}

void ServerInstance::Loop(int32_t result, clock_t lt) {
	clock_t now = clock();
	Calc(static_cast<float>(now - lt)/CLOCKS_PER_SEC);

	state.Set("x", x);
	state.Set("y", y);
	state.Set("pos", pos);
	state.Set("size", size);

	PostMessage(state);

	PostCalc();

	if(!finished) {
		pp::CompletionCallback cc = factory_.NewCallback(&ServerInstance::Loop, now);
		// TODO: Temps d'attente dynamique en fonction du deltatime courant
		msgLoop.PostWork(cc, 1000/60);
	}
}

void ServerInstance::Calc(double deltaTime) {
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
	brickX = x / (100/BRICKW);
	brickY = y / (100/BRICKH);
	brick = briques[brickY][brickX];
	exists = brick > 0;
	breakable = brick < 4;

	if(exists && breakable) {
		briques[brickY][brickX] = brick - 1;
		setBrick(brickY, brickX, brick - 1);
		if(brick == 1) {
			remaining--;
			finished = 1;
			PostMessage(1);
		}
	}

	// TODO: Ajouter des drops (#11)
}

void ServerInstance::PostCalc() {
	if(x >= SCREENSIZE || (exists && prevX < brickX))
		velocity.X = -abs(velocity.X);
	if(x <= 0 || (exists && prevX > brickX))
		velocity.X = abs(velocity.X);
	if(exists && prevY < brickY)
		velocity.Y = -abs(velocity.Y);
	if(y <= 0 || (exists && prevY > brickY))
		velocity.Y = abs(velocity.Y);

	if(y >= SCREENSIZE - 2) {
		if(x >= pos && x <= pos + size) {
			int angle = 45 + (((x - pos)/size) * 90);
			velocity.Y = sin(angle) * BALLSPEED;
			velocity.X = cos(angle) * BALLSPEED;
		} else {
			finished = 1;
			PostMessage(0);
		}
	}

	prevX = brickX;
	prevY = brickY;
}

void ServerInstance::message(pp::Var message) {
	pp::VarDictionary msg(message);
	// Deplacement de la souris
	if(msg.Get("name").AsString() == "m")
		pos += msg.Get("data").AsDouble();
	// Touche enfoncée
	if(msg.Get("name").AsString() == "+")
		inputs[msg.Get("data").AsInt()] = 1;
	// Touche relachée
	if(msg.Get("name").AsString() == "-")
		inputs[msg.Get("data").AsInt()] = 0;
}

void ServerInstance::setBrick(int X, int Y, int valeur) {
	char buffer[10];
	snprintf(buffer, "%d,%d,%d", X, Y, valeur);
	PostMessage(buffer);
}
