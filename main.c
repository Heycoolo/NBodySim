#include <stdio.h>
#include <stdlib.h>
#include "./include/body.h"
#include "./include/system.h"
#include "./include/runge_kutta.h"
#include "./include/anim_sim.h"
#include "./include/parse_input.h"
#include <cjson/cJSON.h>



int main(int argc, char *argv[argc + 1]) {

    char *file_name;
    if (argc == 2) {
        file_name = argv[1];
    } else {
        // Fallback option
        file_name = "./input/figure_eight.json";
        printf("No input file given, falling back to default:\n%s\n", file_name);
    }

    System *sys = parse_input(file_name);
    sys->step_func = RK4;

    initialize_system_state(sys);
    set_initial_system_state(sys);
    update_body_states(sys);
    

    // Time stepping
    for (size_t i = 0; i < sys->n_steps; i++) {
        calculate_next_system_state(sys);
        update_body_states(sys);
        store_body_states(sys, i);
    }


    // Debug
    print_system_state(sys);
    print_body_states(sys);
    size_t sampl = sys->n_steps / 50;
    print_state_array(sys, sampl);


    // Animate
    animate_simulation(sys);

    return 0;
}