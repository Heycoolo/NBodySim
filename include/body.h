#ifndef BODY_H
#define BODY_H

#include <stddef.h>

#define N_DIM 3

// Declaration of system struct (header cannot be included)
typedef struct system System;

typedef double* Position;
typedef double* Velocity;
typedef double* Acceleration;

struct state {
    double pos[N_DIM];
    double vel[N_DIM];
};
struct change {
    double vel[N_DIM];
    double acc[N_DIM];
};

typedef struct state State;
typedef struct change Change;


struct body {
    size_t id;
    size_t n_dim;
    size_t n_steps;
    double mass;
    State init_state;
    State curr_state;
    Change curr_change;
    State *state_arr;
    System *sys;
};

typedef struct body Body;


void initialize_body_state_array(Body *b);
void store_init_body_state(Body *b);
void store_current_body_state(Body *b, size_t curr_step);


#endif