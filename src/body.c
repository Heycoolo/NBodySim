#include "../include/body.h"
#include <stdlib.h>
#include <string.h>



void initialize_body_state_array(Body *b) {


    State *arr = calloc(b->n_steps, sizeof(State));
    if (arr == NULL) {
        exit(1);
    }
    b->state_arr = arr;

    return;
}


void store_init_body_state(Body *b) {

    void *tmp = memcpy(b->state_arr, &(b->init_state), sizeof(State));
    if (tmp == NULL) {
        exit(1);
    }

    return;
}

void store_current_body_state(Body *b, size_t curr_step) {

    if (curr_step > b->n_steps) {
        return;
    }

    void *tmp = memcpy(b->state_arr + curr_step, &(b->curr_state), sizeof(State));
    if (tmp == NULL) {
        exit(1);
    }

    return;
}