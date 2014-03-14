#ifndef MAIN_H
#define MAIN_H

#define SCREENSIZE 40
#define BALLSPEED 40

struct Vector {
    double X;
    double Y;
};

struct Ball {
    struct Vector Position;
    struct Vector Velocity;
};

struct Paddle {
    double Position;
    int Taille;
};

struct Game {
    struct Ball balle;
    struct Paddle paddle;
    int niveau;
};

#endif

