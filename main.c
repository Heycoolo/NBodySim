#include <stdio.h>
#include <stdlib.h>
#include "./include/body.h"
#include "./include/system.h"





int main() {

    // Hardcoded system/bodies and initial condition
    size_t n_body = 3;
    size_t n_dim = 2;
    double init_x[] = {0, 1, 2};
    double init_y[] = {1, 1, -1};
    double init_z[] = {0, 0, 0};
    double init_vx[] = {1, 2, 3};
    double init_vy[] = {2, 2, 2};
    double init_vz[] = {0, 0, 0};
    double masses[] = {1, 1, 1};

    // Time interval
    double t_0 = 0;
    double t_end = 1;
    double step_size = 1e-5;
    size_t n_steps = (t_end - t_0)/step_size;

    // Initialize system
    System sys = {
        .n_bodies = n_body,
        .n_dim = n_dim,
    };


    // Initialize all bodies
    Body *bodies = malloc(n_body * sizeof(Body));
    sys.bodies = bodies;
    for (size_t i = 0; i < n_body; i++) {
        bodies[i] = (Body){
            .id = i,
            .mass = masses[i],
            .n_dim = n_dim,
            .n_steps = n_steps
        };
        initialize_body_state_array(&bodies[i]);
    }





    return 0;
}