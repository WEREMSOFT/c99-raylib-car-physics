#ifndef GAME_H
#define GAME_H

#include <time.h>
#include <string.h>
#include <stdint.h>
#include "physics.h"
#include "car.h"
#include "track.h"

typedef struct game_context_t {
    Camera3D camera;
    Vector3 cube_position;
    Model car;
    car_t car_blue;
    car_t car_red;
    Shader shader;
    Texture2D smoke_texture;
    time_t base_time;
    long double time_spent;
    track_cell_t map[MAP_SIZE_Z][MAP_SIZE_X];
} game_context_t;

void game_init(void);
void game_fini(void);
void game_draw(void);
void game_update(void);
void camera_update(Camera* camera, Vector3 target, float target_offset_y, float position_offset_y);
#endif

#include <raylib.h>

#ifdef GAME_H_IMPLEMENTATION

game_context_t game_context = {0};

static void camera_init(){
    SetCameraMode(game_context.camera, CAMERA_CUSTOM);
    game_context.camera.fovy = 45.0f;
    game_context.camera.target = (Vector3){.0f, .0f, .0f};
    game_context.camera.position = (Vector3){0.0f, 10.0f, 30.0f};
    game_context.camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    game_context.camera.type = CAMERA_PERSPECTIVE;
}

void shader_init(){
    game_context.shader = LoadShader(FormatText("./assets/shaders/glsl%i/base_lighting.vs", GLSL_VERSION),
                            FormatText("./assets/shaders/glsl%i/lighting.fs", GLSL_VERSION));
    game_context.shader.locs[LOC_MATRIX_MODEL] = GetShaderLocation(game_context.shader, "matModel");
    game_context.shader.locs[LOC_VECTOR_VIEW] = GetShaderLocation(game_context.shader, "viewPos");

    int ambientLoc = GetShaderLocation(game_context.shader, "ambient");
    SetShaderValue(game_context.shader, ambientLoc, (float[4]){ 0.2f, 0.2f, 0.2f, 1.0f }, UNIFORM_VEC4);

    Light light_1 = CreateLight(LIGHT_POINT, (Vector3){ -105, 5, -105 }, Vector3Zero(), RED, game_context.shader);
    Light light_2 = CreateLight(LIGHT_POINT, (Vector3){ -105, 5, 105 }, Vector3Zero(), BLUE, game_context.shader);
    Light light_3 = CreateLight(LIGHT_POINT, (Vector3){ 105, 5, -105 }, Vector3Zero(), PURPLE, game_context.shader);
    UpdateLightValues(game_context.shader, light_1);
    UpdateLightValues(game_context.shader, light_2);
    UpdateLightValues(game_context.shader, light_3);

}

void game_init(void){
    game_context.smoke_texture = LoadTexture("assets/smoke2.png");

    shader_init();
    game_context.car = LoadModel("assets/car.obj");
    game_context.car.materials[0].shader = game_context.shader;

    car_init(&game_context.car_red, game_context.car, RED, (unsigned int[]){KEY_UP, KEY_LEFT, KEY_RIGHT});

    car_init(&game_context.car_blue, game_context.car, BLUE, (unsigned int[]){KEY_W, KEY_A, KEY_D});

    game_context.base_time = clock();

    track_init(game_context.map);

    camera_init();
}

void game_fini(void) {
    UnloadShader(game_context.shader);
    UnloadTexture(game_context.smoke_texture);
    UnloadModel(game_context.car);
}

static void draw_track(void) {
    // DrawCube((Vector3){0, -0.2, 0}, 100.0f, 0.01f, 100.0f, GREEN);
    // DrawCube((Vector3){0.0f, 0, 50.0f}, 100.0f, 0.5f, 0.5f, ORANGE);
    // DrawCube((Vector3){0.0f, 0, -50.0f}, 100.0f, 0.5f, 0.5f, ORANGE);
    // DrawCube((Vector3){50.0f, 0, 0.0f}, 0.5f, 0.5f, 100.0f, ORANGE);
    // DrawCube((Vector3){-50.0f, 0, 0.0f}, 0.5f, 0.5f, 100.0f, ORANGE);
    for(int i = 0; i < MAP_SIZE_Z; i++){
        for(int j = 0; j < MAP_SIZE_X; j++){
            if(game_context.map[i][j].enabled){
                Vector3 position = {j * MAP_ZOOM, -.2f, i * MAP_ZOOM};
                DrawCube((Vector3)position, 1.0f * MAP_ZOOM, 0.01f, 1.0f * MAP_ZOOM, GREEN);
            }
        }
    }
}

void camera_update(Camera* camera, Vector3 target, float target_offset_y, float position_offset_y){
    camera->position.x = target.x;
    camera->position.z = target.z + 30.0f;
    
    camera->position.y += position_offset_y;

    target.y = target_offset_y;

    camera->target = target;
    
    UpdateCamera(camera);
}

void game_draw(void){
    ClearBackground(BLACK);

    camera_update(&game_context.camera, game_context.car_blue.particle_head.position, -5.0f, 0.f);
    BeginScissorMode(0, 0, WIDTH, HEIGHT/2);
    BeginMode3D(game_context.camera);
    {
        car_draw(&game_context.car_red);
        car_draw(&game_context.car_blue);
        draw_track();
        // DrawGrid(100, 1);
    }
    EndMode3D();
    EndScissorMode();

    camera_update(&game_context.camera, game_context.car_red.particle_head.position, +8.0f, 0.f);
    BeginScissorMode(0, HEIGHT/2, WIDTH, HEIGHT/2);
    BeginMode3D(game_context.camera);
    {
        car_draw(&game_context.car_red);
        car_draw(&game_context.car_blue);
        draw_track();
    }
    EndMode3D();
    EndScissorMode();
}

void game_update(void){
    printf("car z position: %f\n", game_context.car_red.particle_head.position.z);
    printf("car x position: %f\n", game_context.car_red.particle_head.position.x);
    car_update(&game_context.car_blue, game_context.map);
    car_update(&game_context.car_red, game_context.map);
    
    game_context.camera.fovy += 1.0f * IsKeyDown(KEY_Q);
    game_context.camera.fovy -= 1.0f * IsKeyDown(KEY_E);

    if(IsKeyPressed(KEY_F))
        ToggleFullscreen();
}

#endif