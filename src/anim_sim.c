#include "../include/anim_sim.h"
#include "../include/system.h"
#include "../include/body.h"
#include <raylib.h>
#include <stdio.h>
#include <math.h>



void animate_simulation(System *sys) {

    int FPS = 100;

    const int screenWidth = 1000;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "N-body simulation");
    SetTargetFPS(FPS);

    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0, 0 }; // Point in world space
    // Offset the camera to move (0,0) to the center of the screen
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    State *state;

    int pos_x, pos_y;

    float radius;
    float radius_scale = 1;

    int scl = 50;

    double time_scl = 0.5;

    size_t steps_per_anim_second = 1 / sys->step_size;
    int step_per_frame = steps_per_anim_second/FPS * time_scl;

    char buf[32];
    size_t j = 0;
    while (!WindowShouldClose()) {
        snprintf(buf, 32, "%lu", j);

        BeginDrawing();
        ClearBackground((Color){0,0,0,0});
        BeginMode2D(camera);

        for (size_t i = 0; i < sys->n_bodies; i++) {
            radius = radius_scale * 
                        pow(3 / (4*PI) * (sys->bodies + i)->mass, 1.0/3.0); 
            state = (sys->bodies + i)->state_arr + j;
            pos_x = (int) scl * state->pos.x;
            pos_y = (int) scl * state->pos.y;
            DrawCircle(pos_x, pos_y, radius, (Color){255,255,255,255});
        }
        DrawText(buf, -500, -500, 20, (Color){255,255,255,255});

        EndDrawing();
        j += step_per_frame;
        if (j >= sys->n_steps) {
            j = 0;
        }
    }




    return;
}