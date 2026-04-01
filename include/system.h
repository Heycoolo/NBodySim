#ifndef SYSTEM_H
#define SYSTEM_H

#include "../include/body.h"

struct system {
    size_t n_bodies;
    size_t n_dim;
    double step_size;
    Body * const bodies;
    size_t n_statevariables;
    State *curr_system_state;
};
typedef struct system System;

void initialize_system_state(System *sys);
void set_initial_system_state(System *sys);
void set_current_system_state(System *sys); // I think this is not really needed?!
void calculate_next_system_state(System *sys, State *(*step_func)(System *sys));
void update_body_states(System *sys);
void print_system_state(System *sys);

#endif