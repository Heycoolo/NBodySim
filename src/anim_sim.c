#include "../include/anim_sim.h"
#include "../include/system.h"
#include "../include/body.h"
#include <raylib.h>
#include <stdio.h>



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

    Body *body1 = sys->bodies;
    State *state1 = body1->state_arr;
    Body *body2 = sys->bodies + 1;
    State *state2 = body2->state_arr;
    Body *body3 = sys->bodies + 2;
    State *state3 = body3->state_arr;

    int pos1_x, pos2_x, pos3_x;
    int pos1_y, pos2_y, pos3_y;

    int scl = 50;

    double time_scl = 0.5;

    size_t steps_per_anim_second = 1 / sys->step_size;
    int step_per_frame = steps_per_anim_second/FPS * time_scl;

    char buf[32];
    size_t i = 0;
    while (!WindowShouldClose()) {
        // Update body position
        pos1_x = (int) (scl * state1->pos.x);
        pos1_y = (int) (scl * state1->pos.y);
        pos2_x = (int) (scl * state2->pos.x);
        pos2_y = (int) (scl * state2->pos.y);
        pos3_x = (int) (scl * state3->pos.x);
        pos3_y = (int) (scl * state3->pos.y);


        snprintf(buf, 32, "%lu", i);
        BeginDrawing();
        ClearBackground((Color){0,0,0,0});
        BeginMode2D(camera);
        DrawCircle(pos1_x, pos1_y, 10, (Color){255,255,255,255});
        DrawCircle(pos2_x, pos2_y, 10, (Color){255,255,100,255});
        DrawCircle(pos3_x, pos3_y, 10, (Color){255,100,255,255});
        //DrawCircle(100, 100, 10, (Color){255,255,255,255});
        DrawText(buf, -500, -500, 20, (Color){255,255,255,255});


        EndDrawing();
        i += step_per_frame;
        state1 += step_per_frame;
        state2 += step_per_frame;
        state3 += step_per_frame;
        if (i >= sys->n_steps) {
            i = 0;
            state1 = body1->state_arr;
            state2 = body2->state_arr;
            state3 = body3->state_arr;
        }
    }




    return;
}