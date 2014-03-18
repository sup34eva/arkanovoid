// Copyright 2013 <Huns de Troyes>

#include "jeu.h"

ServerInstance::ServerInstance(PP_Instance instance)
	: pp::Instance(instance), factory_(this) {
	remaining = 0;
	msgLoop = pp::MessageLoop::GetForMainThread();
	rmTime = 1;
	rmLoop = 60;
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
	for(int x = 0; x < BRICKW; x++) {
		for(int y = 0, val; y < BRICKH; y++) {
			val = rand_r() % 5;
			briques.Set((x * BRICKW) + y, val);
			if(val > 0 && val < 4)
				remaining++;
		}
	}

	// brickChanged = 1;

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
	if(brickChanged == 1) {
		state.Set("brick", briques);
		brickChanged = 0;
	}
	state.Set("win", remaining == 0);

	PostMessage(state);

	PostCalc();

	/*rmLoop--;
	if(rmLoop == 0)
		rmLoop = 60;
	rmTime -= double(clock() - lt)/CLOCKS_PER_SEC;
	if(rmTime <= 0.0)
		rmTime = 1;*/

	pp::CompletionCallback cc = factory_.NewCallback(&ServerInstance::Loop, now);
	// TODO: Temps d'attente dynamique en fonction du deltatime courant
	// msgLoop.PostWork(cc, (rmTime * 1000)/rmLoop);
	msgLoop.PostWork(cc, 1000/60);
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
	brick = briques.Get((brickX * BRICKW) + brickY).AsInt();
	exists = brick > 0;
	breakable = brick < 4;

	if(exists && breakable) {
		briques.Set((brickX * BRICKW) + brickY, brick - 1);
		if(brick == 1)
			remaining--;
		// brickChanged = 1;
	}

	// TODO: Ajouter des drops
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

	if(y >= SCREENSIZE - 2 && x >= pos && x <= pos + size) {
		int angle = 45 + (((x - pos)/size) * 90);
		velocity.Y = sin(angle) * BALLSPEED;
		velocity.X = cos(angle) * BALLSPEED;
	}

	prevX = brickX;
	prevY = brickY;
}

void ServerInstance::message(pp::Var message) {
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
