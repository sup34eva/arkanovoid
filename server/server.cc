#include "jeu.h"

ServerInstance::ServerInstance(PP_Instance instance) : pp::Instance(instance), factory_(this) {
	remaining = 0;
	msgLoop = pp::MessageLoop::GetForMainThread();
}

ServerInstance::~ServerInstance() {}

void ServerInstance::HandleMessage(const pp::Var& var_message) {
	if(var_message.is_dictionary())
		message(var_message);
	#ifndef MSGLOOP
	if(var_message.is_number())
		Loop(var_message.AsDouble());
	#endif
}

bool ServerInstance::Init ( uint32_t argc, const char * argn[], const char * argv[]) {
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

	#ifdef MSGLOOP
	Loop(0, clock());
	#endif

	return true;
}

#ifdef MSGLOOP
void ServerInstance::Loop (int32_t result, clock_t lt) {
	double dt = ((double)(clock() - lt))/CLOCKS_PER_SEC;
	pp::CompletionCallback cc;

	if(dt >= 1/60) {
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

		cc = factory_.NewCallback(&ServerInstance::Loop, clock());
	} else
		cc = factory_.NewCallback(&ServerInstance::Loop, lt);

	msgLoop.PostWork(cc);
}
#else
void ServerInstance::Loop (double dt) {
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
#endif

void ServerInstance::Calc (double deltaTime) {
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

void ServerInstance::PostCalc() {
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

void ServerInstance::message (pp::Var message) {
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
