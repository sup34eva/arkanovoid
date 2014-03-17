#ifndef MAIN_H
#define MAIN_H

#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/var_dictionary.h"
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
    pp::VarArray briques;
};

#endif

