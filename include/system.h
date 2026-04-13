#ifndef SYSTEM_H
#define SYSTEM_H

#include "../include/body.h"

struct system {
    size_t n_bodies;
    size_t n_steps;
    double step_size;
    size_t n_statevariables;
    State *curr_system_state;
    State *next_system_state;
    void (*eval_func)(Change *k, State *state, System *sys);
    void (*step_func)(System *sys);
    Body * bodies;
};
typedef struct system System;

void initialize_system_state(System *sys);
void set_initial_system_state(System *sys);

void calculate_next_system_state(System *sys);
void update_body_states(System *sys);
void store_body_states(System *sys, size_t curr_step);

void evaluate_force_law(Change *k, State *state, System *sys);

// Debugging functions
void print_system_state(System *sys);
void print_body_states(System *sys);
void print_state_array(System *sys, size_t sampl);

#endif