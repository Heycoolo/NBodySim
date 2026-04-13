#include "../include/system.h"
#include "../include/body.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#define NUMERICAL_THRESHHOLD_D 1e-12
#define N_DIM 3
#define GRAVIT 6.7e-3


// Function declarations
void zero_change_of_state(System *sys);
double calculate_distance(Position pos1, Position pos2);




void initialize_system_state(System *sys) {

    State *arr = calloc(sys->n_bodies, sizeof(State));
    if (arr == NULL) {
        exit(1);
    }

    sys->curr_system_state = arr;

    return;
}


void set_initial_system_state(System *sys) {

    Body tmp_body;
    State tmp_state;

    for (size_t i = 0; i < sys->n_bodies; i++) {
        tmp_body = sys->bodies[i];
        tmp_state = tmp_body.init_state;
        void *tmp = memcpy(sys->curr_system_state + i,
                            &tmp_state,
                            sizeof(State));
        if (tmp == NULL) {
            exit(1);
        }
    }

    return;
}


void update_body_states(System *sys) {

    for (size_t i = 0; i < sys->n_bodies; i++) {        
        void *tmp = memcpy(&(sys->bodies + i)->curr_state,
                            sys->curr_system_state + i,
                            sizeof(State));
        if (tmp == NULL) {
            exit(1);
        }
    }

    return;
}

void store_body_states(System *sys, size_t curr_step) {

    if (curr_step > sys->n_steps) {
        return;
    }
    Body *b;
    for (size_t i = 0; i < sys->n_bodies; i++) {
        b = sys->bodies + i;
        store_current_body_state(b, curr_step);
    }


    return;
}


void calculate_next_system_state(System *sys) {

    sys->step_func(sys);
    return;
}


void evaluate_force_law(Change *k, State *state, System *sys) {

    zero_change_of_state(sys);

    double distance;
    double tmp_a;
    
    Body *tmp_body1 = sys->bodies;
    Body *tmp_body2;

    State *state_1 = state;
    State *state_2 = state;
    
    for (size_t i = 0; i < sys->n_bodies; i++) {
        // Assign current velocities to the state change
        for (size_t j = 0; j < N_DIM; j++) {
            tmp_body1->curr_change.vel[j] = state_1->vel[j];
        }

        for (size_t j = 0; j < sys->n_bodies; j++) {
            if (i == j) {
                continue;
            }
            state_2 = state + j;
            tmp_body2 = sys->bodies + j;
            distance = calculate_distance(state_1->pos, state_2->pos);
            if (fabs(distance) <= NUMERICAL_THRESHHOLD_D) {
                continue;
            }
            
            // Add to new state of body i
            for (size_t j = 0; j < N_DIM; j++) {
                tmp_a = GRAVIT * tmp_body2->mass / (pow(distance, 3))
                        * (state_2->pos[j] - state_1->pos[j]);
                tmp_body1->curr_change.acc[j] += tmp_a;
            }
        }

        void *tmp = memcpy(k + i,
                            &tmp_body1->curr_change,
                            sizeof(Change));
        if (tmp == NULL) {
            exit(1);
        }

        tmp_body1++;
        state_1++;
    }
    return;
}


void zero_change_of_state(System *sys) {
    Body *b = sys->bodies;
    for (size_t i = 0; i < sys->n_bodies; i++) {
        for (size_t j = 0; j < N_DIM; j++) {
            b->curr_change.vel[j] = 0;
            b->curr_change.acc[j] = 0;
        }
        b++;
    }
    return;
}


double calculate_distance(Position pos1, Position pos2) {

    double r = 0;
    for (size_t j = 0; j < N_DIM; j++) {
        r += (pos2[j] - pos1[j]) * (pos2[j] - pos1[j]);
    }

    r = sqrt(r);
    return r;
}





// Debugging function
void print_system_state(System *sys) {

    printf("\nCurrent system state is defined by all bodies\n");
    State tmp_state;
    for (size_t i = 0; i < sys->n_bodies; i++) {
        tmp_state = sys->curr_system_state[i];
        printf("Body %lu:\tpos: (%lf, %lf, %lf)\tvel: (%lf, %lf, %lf)\n", i,
                                    tmp_state.pos[0],
                                    tmp_state.pos[1],
                                    tmp_state.pos[2],
                                    tmp_state.vel[0],
                                    tmp_state.vel[1],
                                    tmp_state.vel[2]);
    }

    return;
}

void print_body_states(System *sys) {

    printf("\nCurrent system changes are defined by force law\n");
    Change *tmp_change;
    Body *b;
    for (size_t i = 0; i < sys->n_bodies; i++) {
        b = sys->bodies + i;
        tmp_change = &b->curr_change;
        printf("Body %lu: \tvel: (%lf, %lf, %lf) \tacc: (%lf, %lf, %lf)\n", i,
                                    tmp_change->vel[0],
                                    tmp_change->vel[1],
                                    tmp_change->vel[2],
                                    tmp_change->acc[0],
                                    tmp_change->acc[1],
                                    tmp_change->acc[2]);
    }

    return;
}

void print_state_array(System *sys, size_t sampl) {

    Body *b;
    State *tmp_state;
    printf("\nDisplaying state every %lu steps\n", sampl);
    for (size_t i = 0; i < sys->n_steps; i += sampl) {
        printf("Time: %lfs\n", sys->step_size * i);
        for (size_t j = 0; j < sys->n_bodies; j++) {
            b = sys->bodies + j;
            tmp_state = b->state_arr + i;
            printf("Body %lu:\tpos: (%lf, %lf, %lf)\tvel: (%lf, %lf, %lf)\n", j,
                                    tmp_state->pos[0],
                                    tmp_state->pos[1],
                                    tmp_state->pos[2],
                                    tmp_state->vel[0],
                                    tmp_state->vel[1],
                                    tmp_state->vel[2]);
        }
    }

    return;
}