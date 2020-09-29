#include <stdio.h>
#include <raylib.h>
#include <time.h>

#ifdef OS_WEB
#include <emscripten/emscripten.h>
#endif

#define PHYSICS_H_IMPLEMENTATION
#include "physics.h"

#define WIDTH 800
#define HEIGHT 600

#if defined(OS_WEB)
#define GLSL_VERSION            100
#else   // PLATFORM_WEB
#define GLSL_VERSION            330
#endif

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#define CAR_H_IMPLEMENTATION
#include "car.h"

#define GAME_H_IMPLEMENTATION
#include "game.h"

#define TRACK_H_IMPLEMENTATION
#include "track.h"

#include "queue.h"

void update_frame()
{
    game_update();

    BeginDrawing();
    {
        game_draw();
    }
    DrawFPS(710, 10);
    DrawText("W: accelerate, A/D: Turn", 10, 10, 20, BLUE);
    DrawText("UP ARROW: accelerate, LEFT/RIGHT: Turn", 10, 310, 20, RED);
    EndDrawing();
}

int main(void)
{
#ifdef OS_Windows_NT
    printf("Windows dettected\n");
#elif defined OS_Linux
    printf("LINUS dettected\n");
#elif defined OS_Darwin
    printf("MacOS dettected\n");
#endif

    InitWindow(WIDTH, HEIGHT, "This is a network test");
    SetTargetFPS(60);

    game_init();

#ifdef OS_WEB
    emscripten_set_main_loop(update_frame, 0, 1);
#else
    while (!WindowShouldClose())
    {
        update_frame();
    }
#endif
    game_fini();
    CloseWindow();


    return 0;
}