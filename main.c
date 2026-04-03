#include <stdio.h>
#include <stdlib.h>
#include "./include/body.h"
#include "./include/system.h"
#include "./include/runge_kutta.h"
#include "./include/anim_sim.h"




int main() {

    // Hardcoded system/bodies and initial condition
    size_t n_body = 3;
    size_t states_per_body = 2 * 3; //3 pos + 3 vel
    size_t n_dim = 2;
    double init_x[] = {0, 3, 0};
    double init_y[] = {0, 0, 3};
    double init_z[] = {0, 0, 0};
    double init_vx[] = {0, 0, -5};
    double init_vy[] = {0, 5, -1};
    double init_vz[] = {0, 0, 0};
    double masses[] = {4e4, 1e2, 1e2};

    // Time interval
    double t_0 = 0;
    double t_end = 10;
    double step_size = 1e-6;
    size_t n_steps = (t_end - t_0)/step_size;

    // Allocate space for body structs
    Body * const bodies = calloc(n_body, sizeof(Body));


    // Initialize system
    System sys = {
        .n_bodies = n_body,
        .n_dim = n_dim,
        .n_steps = n_steps,
        .step_size = step_size,
        .bodies = bodies,
        .n_statevariables = n_body * states_per_body,
        .eval_func = evaluate_force_law,
        .step_func = RK4
    };


    // Initialize all bodies
    Position tmp_pos;
    Velocity tmp_vel;
    Body tmp_body;
    for (size_t i = 0; i < n_body; i++) {
        tmp_pos = (Position){
            .x = init_x[i],
            .y = init_y[i],
            .z = init_z[i]
        };
        tmp_vel = (Velocity){
            .vx = init_vx[i],
            .vy = init_vy[i],
            .vz = init_vz[i]
        };
        bodies[i] = (Body){
            .id = i,
            .mass = masses[i],
            .n_dim = n_dim,
            .n_steps = n_steps,
            .init_state = (State) {.pos = tmp_pos, .vel = tmp_vel},
        };
        initialize_body_state_array(bodies + i);
        store_init_body_state(bodies + i);
        tmp_body = *(bodies + i);
        printf("x: %lf, y: %lf, vx: %lf, vy: %lf\n",
                                                tmp_body.state_arr->pos.x,
                                                tmp_body.state_arr->pos.y,
                                                tmp_body.state_arr->vel.vx,
                                                tmp_body.state_arr->vel.vy);
    }
    initialize_system_state(&sys);
    set_initial_system_state(&sys);
    update_body_states(&sys);
    

    // Time stepping
    for (size_t i = 0; i < n_steps; i++) {
        calculate_next_system_state(&sys);
        update_body_states(&sys);
        store_body_states(&sys, i);
    }


    // Debug
    print_system_state(&sys);
    print_body_states(&sys);
    size_t sampl = sys.n_steps / 50;
    print_state_array(&sys, sampl);


    // Animate
    animate_simulation(&sys);

    free(bodies);
    return 0;
}