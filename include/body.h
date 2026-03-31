#ifndef BODY_H
#define BODY_H

// This constant is needed for memory allocation inside the struct
// Fewer dimensions are allowed, then the 3rd coordinate is neglected
#define MAXIMAL_DIMENSIONS 3


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
    State *sol_arr;
};
typedef struct body Body;


void initialize_body_state_array(Body *b);



#endif