#ifndef BODY_H
#define BODY_H


//#include "../include/system.h"
#include <stddef.h>

struct position {
    double x;
    double y;
    double z;
};
struct velocity {
    double vx;
    double vy;
    double vz;
};
typedef struct position Position;
typedef struct velocity Velocity;

struct state {
    Position pos;
    Velocity vel;
};
typedef struct state State;

struct body {
    size_t id;
    size_t n_dim;
    size_t n_steps;
    double mass;
    State init_state;
    State curr_state;
    State *state_arr;
    //System *sys;
};
typedef struct body Body;


void initialize_body_state_array(Body *b);
void store_init_body_state(Body *b);
void store_current_body_state(Body *b, size_t curr_step);


#endif