#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include "main.h"

double min(double a, double b) {
	if(a > b)
		return b;
	else
		return a;
}

double max(double a, double b) {
	if(a < b)
		return b;
	else
		return a;
}

double getTime() {
    struct timeval v;
    gettimeofday(&v, NULL);
    return v.tv_usec;
}

/*int collides(struct Ball a, struct Paddle b) {
  return a.x < b.x + b.width &&
         a.x + a.width > b.x &&
         a.y < b.y + b.height &&
         a.y + a.height > b.y;
}*/

int main() {
    double t = 0.0;
    const double dt = 0.01;

    double currentTime = getTime();
    double accumulator = 0.0;

	struct Game state;
	init(&state);

	while(1) {
		double newTime = getTime();
        double frameTime = newTime - currentTime;
        currentTime = newTime;

        accumulator += frameTime;
		
        while ( accumulator >= dt )
        {
            calc(t, dt, &state);
            draw(&state);
            accumulator -= dt;
            t += dt;
		}
	}

	return 0;
}

/* void init (Ball*, Paddle*)
 * Cette fonction executée une fois au démarrage du programme initialise toute les variables */
void init (struct Game* state) {
	// Ces variables contiennent la vitesse de la balle sur les deux axes en unités par secondes
	srand(time(NULL));
	state->ball.Velocity.X = rand() % BALLSPEED;
	state->ball.Velocity.Y = rand() % BALLSPEED;
	state->ball.Position.X = 0;
	state->ball.Position.Y = 0;
	state->paddle.Position = 0;
	state->paddle.Taille = 5;
}

/* void calc (Ball*, Paddle*)
 * Cette fonction, executée 60 fois par secondes environ, actualise l'état de la balle et de la paddle */
void calc (double time, double deltaTime, struct Game* state) {
	state->ball.Position.X += state->ball.Velocity.X * deltaTime;
	state->ball.Position.Y += state->ball.Velocity.Y * deltaTime;
	// TODO: A remplacer par une position calculée par rapport aux events clavier/souris
	state->paddle.Position = state->ball.Position.Y - state->paddle.Taille/2;
	state->paddle.Position = min(max(state->paddle.Position, 0), SCREENSIZE - state->paddle.Taille);

	// Gestion des collisions basique
	if(state->ball.Position.X >= SCREENSIZE)
		state->ball.Velocity.X = -abs(state->ball.Velocity.X);
	if(state->ball.Position.X <= 0)
		state->ball.Velocity.X = abs(state->ball.Velocity.X);
	if(state->ball.Position.Y >= SCREENSIZE)
		state->ball.Velocity.Y = -abs(state->ball.Velocity.Y);
	if(state->ball.Position.Y <= 0)
		state->ball.Velocity.Y = abs(state->ball.Velocity.Y);
}


/* void handleMessage(char*)
 * Reçoit les messages du client (evenements du clavier et de la souris) */
void handleMessage(char* msg) {
        // TODO: Ajouter le support des entrées du joueur
}  

/* void draw (Ball*, Paddle*)
 * Cette fonction un peu complexe affiche la zone de jeu a l'écran.
 * Son fonctionnement est peu interressant et elle ne seras pas la dans la version finale
 * (puisqu'a la fin c'est le client se chargera de l'affichage)
 * Elle n'est donc utile que pour déboguer le programme. */
void draw (struct Game* state) {
	char buffer[(SCREENSIZE*SCREENSIZE) + (SCREENSIZE*6) + 6] = "";
	for(int i = 0; i <= SCREENSIZE + 1; i++) {
		for(int j = 0; j <= SCREENSIZE+1; j++) {
			if((i == 0 || i == SCREENSIZE+1) && (j == 0 || j == SCREENSIZE+1))
				strcat(buffer, "+");
			else if(i == 0 || i == SCREENSIZE+1)
				strcat(buffer, "-");
			else if(j == 0 || j == SCREENSIZE+1)
				strcat(buffer, "|");
			else if(i == (int)(state->ball.Position.X + 1) && j == (int)(state->ball.Position.Y + 1))
				strcat(buffer, "*");
			else if(i == SCREENSIZE && j >= (int)(state->paddle.Position + 1) && j <= (int)(state->paddle.Position + state->paddle.Taille + 1))
				strcat(buffer, "=");
			else
				strcat(buffer, " ");
		}
		strcat(buffer, "\n");
	}
#ifdef __MINGW32__
	system("CLS");
#else
	system("clear");
#endif
	printf("%s", buffer);
}

