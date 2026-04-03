#include "../include/system.h"
#include "../include/body.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#define NUMERICAL_THRESHHOLD_D 1e-12
//#define GRAVIT 6.6743e-11
#define GRAVIT 6.7e-3


// Function declarations
void zero_change_of_state(System *sys);
double calculate_distance(Position *pos1, Position *pos2);




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
    Body *tmp_body1;
    Body *tmp_body2;
    double tmp_ax;
    double tmp_ay;
    double tmp_az;
    for (size_t i = 0; i < sys->n_bodies; i++) {
        tmp_body1 = sys->bodies + i;
        
        // Assign current velocities to the state change
        tmp_body1->curr_change.vel.vx = tmp_body1->curr_state.vel.vx;
        tmp_body1->curr_change.vel.vy = tmp_body1->curr_state.vel.vy;
        tmp_body1->curr_change.vel.vz = tmp_body1->curr_state.vel.vz;

        for (size_t j = 0; j < sys->n_bodies; j++) {
            if (i == j) {
                continue;
            }
            tmp_body2 = sys->bodies + j;
            distance = calculate_distance(&tmp_body1->curr_state.pos,
                                          &tmp_body2->curr_state.pos);
            if (fabs(distance) <= NUMERICAL_THRESHHOLD_D) {
                continue;
            }
            tmp_ax = GRAVIT * tmp_body2->mass /(pow(distance, 3))
                    * (tmp_body2->curr_state.pos.x - tmp_body1->curr_state.pos.x);
            tmp_ay = GRAVIT * tmp_body2->mass /(pow(distance, 3))
                    * (tmp_body2->curr_state.pos.y - tmp_body1->curr_state.pos.y);
            tmp_az = GRAVIT * tmp_body2->mass /(pow(distance, 3))
                    * (tmp_body2->curr_state.pos.z - tmp_body1->curr_state.pos.z);
            // Add to new state of body i
            tmp_body1->curr_change.acc.ax += tmp_ax;
            tmp_body1->curr_change.acc.ay += tmp_ay;
            tmp_body1->curr_change.acc.az += tmp_az;
        }
    }

    for (size_t i = 0; i < sys->n_bodies; i++) {
        void *tmp = memcpy(k + i, &(sys->bodies + i)->curr_change, sizeof(Change));
    }

    return;
}


void zero_change_of_state(System *sys) {
    Body *b = sys->bodies;
    for (size_t i = 0; i < sys->n_bodies; i++) {
        b->curr_change.vel.vx = 0;
        b->curr_change.vel.vy = 0;
        b->curr_change.vel.vz = 0;
        b->curr_change.acc.ax = 0;
        b->curr_change.acc.ay = 0;
        b->curr_change.acc.az = 0;
        b++;
    }
    return;
}


double calculate_distance(Position *pos1, Position *pos2) {

    double dx = pos2->x - pos1->x;
    double dy = pos2->y - pos1->y;
    double dz = pos2->z - pos1->z;

    double r = sqrt(dx*dx + dy*dy + dz*dz);
    return r;
}





// Debugging function
void print_system_state(System *sys) {

    printf("\nCurrent system state is defined by all bodies\n");
    State tmp_state;
    for (size_t i = 0; i < sys->n_bodies; i++) {
        tmp_state = sys->curr_system_state[i];
        printf("Body %lu:\tpos: (%lf, %lf, %lf)\tvel: (%lf, %lf, %lf)\n", i,
                                    tmp_state.pos.x,
                                    tmp_state.pos.y,
                                    tmp_state.pos.z,
                                    tmp_state.vel.vx,
                                    tmp_state.vel.vy,
                                    tmp_state.vel.vz);
    }

    return;
}

void print_body_states(System *sys) {

    printf("\nCurrent system changes are defined by force law\n");
    State *tmp_state;
    Change *tmp_change;
    Body *b;
    for (size_t i = 0; i < sys->n_bodies; i++) {
        b = sys->bodies + i;
        tmp_change = &b->curr_change;
        printf("Body %lu: \tvel: (%lf, %lf, %lf) \tacc: (%lf, %lf, %lf)\n", i,
                                    tmp_change->vel.vx,
                                    tmp_change->vel.vy,
                                    tmp_change->vel.vz,
                                    tmp_change->acc.ax,
                                    tmp_change->acc.ay,
                                    tmp_change->acc.az);
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
                                    tmp_state->pos.x,
                                    tmp_state->pos.y,
                                    tmp_state->pos.z,
                                    tmp_state->vel.vx,
                                    tmp_state->vel.vy,
                                    tmp_state->vel.vz);    
        }
    }

    return;
}