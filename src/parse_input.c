#include "../include/parse_input.h"
#include "../include/system.h"
#include "../include/body.h"
#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>


// Function declarations
static cJSON* read_file(const char *file_name);
static void fill_sys_struct(System *sys, cJSON *system_json);
static void fill_integrator(System *sys, cJSON *integrator_json);
static void fill_bodies(System *sys, cJSON *bodies_json);


System* parse_input(char *file_name) {

    System *sys = calloc(1, sizeof(System));
    if (sys == NULL) {
        exit(1);
    }

    cJSON *system_json = read_file(file_name);

    fill_sys_struct(sys, system_json);

    return sys;
}


static cJSON* read_file(const char *file_name) {

    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    size_t total_chars = 0;
    char tmp_char;
    while ((tmp_char = fgetc(file)) != EOF) {
        total_chars++;
    }

    rewind(file);
    char *file_string = calloc(total_chars, sizeof(char));
    size_t i = 0;
    while ((tmp_char = fgetc(file)) != EOF) {
        file_string[i] = tmp_char;
        i++;
    }

    cJSON *system_json = cJSON_Parse(file_string);

    free(file_string);
    fclose(file);
    return system_json;
}


static void fill_sys_struct(System *sys, cJSON *system_json) {

    // Parse integrator input
    cJSON *integrator_json = cJSON_GetObjectItemCaseSensitive(system_json,
                                                              "integrator");
    if (integrator_json == NULL) {
        exit(1);
    }

    fill_integrator(sys, integrator_json);

    // Parse body input
    cJSON *bodies_json = cJSON_GetObjectItemCaseSensitive(system_json, "bodies");
    if (bodies_json == NULL) {
        exit(1);
    }

    fill_bodies(sys, bodies_json);
    
    return;
}


static void fill_integrator(System *sys, cJSON *integrator_json) {

    size_t n_steps;
    cJSON *i_step = cJSON_GetObjectItemCaseSensitive(integrator_json, "time_step");
    cJSON *i_total_time = cJSON_GetObjectItemCaseSensitive(integrator_json, "total_time");
    n_steps = i_total_time->valuedouble / i_step->valuedouble;

    sys->n_steps = n_steps;
    sys->step_size = i_step->valuedouble;
    sys->eval_func = evaluate_force_law;
    
    return;
}

static void fill_bodies(System *sys, cJSON *bodies_json) {

    size_t n_bodies = cJSON_GetArraySize(bodies_json);
    if (n_bodies == 0) {
        printf("No bodies found!\n");
        exit(1);
    }

    Body *bodies = calloc(n_bodies, sizeof(Body));
    if (bodies == NULL) {
        perror("Allocation failed");
        exit(1);
    }
    sys->bodies = bodies;
    sys->n_bodies = n_bodies;
    sys->n_statevariables = n_bodies * 2 * N_DIM;
    cJSON *b_id;
    cJSON *b_mass;
    cJSON *curr_body;
    cJSON *b_position;
    cJSON *b_velocity;

    for (size_t i = 0; i < n_bodies; i++) {
        curr_body = cJSON_GetArrayItem(bodies_json, i);
        b_id = cJSON_GetObjectItemCaseSensitive(curr_body, "id");
        b_mass = cJSON_GetObjectItemCaseSensitive(curr_body, "mass");
        b_position = cJSON_GetObjectItemCaseSensitive(curr_body, "position");
        b_velocity = cJSON_GetObjectItemCaseSensitive(curr_body, "velocity");
        for (size_t j = 0; j < N_DIM; j++) {
            bodies->init_state.pos[j] = cJSON_GetArrayItem(b_position, j)->valuedouble;
            bodies->init_state.vel[j] = cJSON_GetArrayItem(b_velocity, j)->valuedouble;
        }
        bodies->id = b_id->valueint;
        bodies->mass = b_mass->valuedouble;
        bodies->n_steps = sys->n_steps;
        initialize_body_state_array(bodies);
        store_init_body_state(bodies);
        bodies++;
    }


    return;
}