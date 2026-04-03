#ifndef BODY_H
#define BODY_H

#include <stddef.h>

// Declaration of system struct (header cannot be included)
typedef struct system System;


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
struct acceleration {
    double ax;
    double ay;
    double az;
};

typedef struct position Position;
typedef struct velocity Velocity;
typedef struct acceleration Acceleration;


struct state {
    Position pos;
    Velocity vel;
};
struct change {
    Velocity vel;
    Acceleration acc;
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