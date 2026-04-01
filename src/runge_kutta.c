#include "../include/runge_kutta.h"
#include "../include/system.h"



void RK4(System * const sys,
            void (*eval_func)(State *dest, State *curr_state, System *sys)) {

    double step_size = sys->step_size;

    State *curr_state = sys->curr_system_state;
    State *interim_state = calloc(sys->n_bodies, sizeof(State));
    State *k1 = calloc(sys->n_bodies, sizeof(State));
    State *k2 = calloc(sys->n_bodies, sizeof(State));
    State *k3 = calloc(sys->n_bodies, sizeof(State));
    State *k4 = calloc(sys->n_bodies, sizeof(State));
    State *new_state = calloc(sys->n_bodies, sizeof(State));

    // k1 = f( yn )
    eval_func(k1, curr_state, sys);
    

    // k2 = f( yn + k1 * step_size/2 )
    add_states(interim_state, curr_state, k1, step_size/2, sys->n_statevariables);
    eval_func(k2, interim_state, sys);
    
    // k3 = f( yn + k2 * step_size/2 )
    add_states(interim_state, curr_state, k2, step_size/2, sys->n_statevariables);
    eval_func(k3, interim_state, sys);

    // k4 = f( yn + k3 * step_size )
    add_states(interim_state, curr_state, k3, step_size, sys->n_statevariables);
    eval_func(k4, interim_state, sys);

    size_t total_len = sys->n_bodies * sizeof(State)/sizeof(double);
    add_states(new_state, curr_state, k1, 0, total_len);
    add_states(new_state, new_state, k1, step_size/6, total_len);
    add_states(new_state, new_state, k2, step_size/3, total_len);
    add_states(new_state, new_state, k3, step_size/3, total_len);
    add_states(new_state, new_state, k4, step_size/6, total_len);
    void *tmp = memcpy(sys->curr_system_state, new_state, sys->n_bodies * sizeof(State));
    if (tmp == NULL) {
        exit(1);
    }

    free(interim_state);
    free(k1);
    free(k2);
    free(k3);
    free(k4);
    free(new_state);
    return;
}


static void add_states(State *interim_state, State *curr_state, State *k,
                        double factor, size_t total_len) {

    double *tmp_interim = (double*) interim_state;
    double *tmp_curr = (double*) curr_state;
    double *tmp_k = (double*) k;
    for (size_t i = 0; i < total_len; i++) {
        *(tmp_interim + i) = *(tmp_curr + i) + factor * *(tmp_k + i);
    }

    return;
}

static void multiply_scalar(double factor, State *state, size_t arr_len) {
    double *tmp = (double*) state;
    for (size_t i = 0; i < arr_len; i++) {
        *(tmp + i) *= factor;
    }
    return;
}