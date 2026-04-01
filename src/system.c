#include "../include/system.h"
#include "../include/body.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



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


void set_current_system_state(System *sys) {

    Body tmp_body;
    State tmp_state;
    for (size_t i = 0; i < sys->n_bodies; i++) {
        tmp_body = sys->bodies[i];
        tmp_state = tmp_body.curr_state;
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

    Body tmp_body;
    State tmp_state;
    for (size_t i = 0; i < sys->n_bodies; i++) {
        tmp_body = sys->bodies[i];
        
        void *tmp = memcpy(&(sys->bodies + i)->curr_state,
                            sys->curr_system_state + i,
                            sizeof(State));
        if (tmp == NULL) {
            exit(1);
        }
    }

    return;
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