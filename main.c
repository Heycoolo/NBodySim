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


    // Figure eight -- Periodic solution
    double dist_scale = 4;
    double init_x[] = {dist_scale*0.97000436, dist_scale*-0.97000436, 0};
    double init_y[] = {dist_scale*-0.24308753, dist_scale*0.24308753, 0};
    double init_z[] = {0, 0, 0};
    double init_vx[] = {dist_scale*0.466203685012, dist_scale*0.466203685012, dist_scale*-0.932407370024};
    double init_vy[] = {dist_scale*0.432365730026, dist_scale*0.432365730026, dist_scale*-0.864731460052};
    double init_vz[] = {0, 0, 0};
    double masses[] = {1e4, 1e4, 1e4};

    //double init_x[] = {0, 3, -6};
    //double init_y[] = {0, 0, 0};
    //double init_z[] = {0, 0, 0};
    //double init_vx[] = {0, 0, 0};
    //double init_vy[] = {0, 10, -5};
    //double init_vz[] = {0, 0, 0};
    //double masses[] = {4e4, 1e3, 1e3};


    // Time interval
    double t_0 = 0;
    double t_end = 20;
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
    State tmp_state;
    double tmp_pos[3];
    double tmp_vel[3];
    //Body tmp_body;
    for (size_t i = 0; i < n_body; i++) {
        //tmp_pos = {init_x[i], init_y[i], init_z[i]};
        //tmp_vel = {init_vx[i], init_vy[i], init_vz[i]};
        tmp_state = (State){.pos = {init_x[i], init_y[i], init_z[i]},
                .vel = {init_vx[i], init_vy[i], init_vz[i]}};
        bodies[i] = (Body){
            .id = i,
            .mass = masses[i],
            .n_dim = n_dim,
            .n_steps = n_steps,
            .init_state = tmp_state,
        };
        initialize_body_state_array(bodies + i);
        store_init_body_state(bodies + i);
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