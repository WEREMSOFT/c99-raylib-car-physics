#include <stdio.h>
#include <raylib.h>
#include <time.h>

#ifdef OS_WEB
#include <emscripten/emscripten.h>
#endif

#define __INCLUDED_IN_MAIN__
#include "physics.h"

#define WIDTH 800
#define HEIGHT 600
#define PARTICLE_COUNT 2

#if defined(OS_WEB)
#define GLSL_VERSION            100
#else   // PLATFORM_WEB
#define GLSL_VERSION            330
#endif

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include "queue.h"

Camera3D camera = {0};
Vector3 cube_position = {0.0f, 0.0f, 0.0f};
particle_t particles[PARTICLE_COUNT] = {0};
Vector3 acceleration_vector = {0, 0, .01f};
Quaternion direction = {0};
float angle = 0;
Model car = {0};
Shader shader = {0};
Texture2D smoke_texture = {0};
time_t base_time = {0};
long double time_spent = 0.0f;

void update_frame()
{
    camera.position.x = particles[0].position.x + 30.0f;
    camera.position.z = particles[0].position.z;
    UpdateCamera(&camera);

    camera.target = particles[0].position;

    for(int i = 0; i < PARTICLE_COUNT; i++){
        if(i == 0)
            particle_update(&particles[i], 0.95);
        else
            particle_update(&particles[i], 0.96);

        if(i < PARTICLE_COUNT-1){
            particle_fix_distance(&particles[i], &particles[i+1]);
        }
        particle_restrict(&particles[i], 48.5f);
    }
    

    BeginDrawing();
    {

        ClearBackground(BLACK);
        DrawFPS(10, 10);
        DrawText("UP ARROW: accelerate, LEFT/RIGHT: Turn", 10, 50, 20, RED);

        BeginMode3D(camera);
        {
            time_t t = clock();
            time_spent += (double)(t - base_time) / CLOCKS_PER_SEC;

            if(time_spent > 1.0f){
                vector3_queue_enqueue(particles[1].position);
                time_spent = 0.0f;
            }
            int queue_size = vector3_queue_get_size();

            if(queue_size > 10)
                vector3_queue_dequeue();

            for(int i = 0; i < queue_size; i++){
                DrawBillboard(camera, smoke_texture, vector3_queue_get_element_at(i), 1.0f, WHITE);
            }
            car.transform = MatrixRotateY(Vector2Angle((Vector2){particles[1].position.x, particles[1].position.z}, (Vector2){particles[0].position.x, particles[0].position.z}) / RAD2DEG);
            DrawModel(car, particles[0].position, 0.5f, WHITE);

            DrawCube((Vector3){0, -0.2, 0}, 100.0f, 0.01f, 100.0f, GREEN);
            DrawCube((Vector3){0.0f, 0, 50.0f}, 100.0f, 0.5f, 0.5f, ORANGE);
            DrawCube((Vector3){0.0f, 0, -50.0f}, 100.0f, 0.5f, 0.5f, ORANGE);
            DrawCube((Vector3){50.0f, 0, 0.0f}, 0.5f, 0.5f, 100.0f, ORANGE);
            DrawCube((Vector3){-50.0f, 0, 0.0f}, 0.5f, 0.5f, 100.0f, ORANGE);

            DrawGrid(100, 1);
        }
        EndMode3D();

        if (IsKeyDown(KEY_KP_ADD))
            camera.fovy += 1.0f;
        if (IsKeyDown(KEY_KP_SUBTRACT))
            camera.fovy -= 1.0f;

        acceleration_vector = Vector3Zero();

        if (IsKeyDown(KEY_UP))
            acceleration_vector.z += 0.04f;
        if (IsKeyDown(KEY_LEFT))
            angle += 0.05f;
        if (IsKeyDown(KEY_RIGHT))
            angle -= 0.05f;
        
        direction = QuaternionFromAxisAngle((Vector3){0, 1.0f, 0}, angle);

        acceleration_vector = Vector3RotateByQuaternion(acceleration_vector, direction);

        particles[0].position = Vector3Add(particles[0].position, acceleration_vector);

    }
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

    base_time = clock();
    InitWindow(WIDTH, HEIGHT, "This is a network test");
    SetTargetFPS(60);

    shader = LoadShader(FormatText("./assets/shaders/glsl%i/base_lighting.vs", GLSL_VERSION),
                               FormatText("./assets/shaders/glsl%i/lighting.fs", GLSL_VERSION));

    smoke_texture = LoadTexture("assets/smoke2.png");

    shader.locs[LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");
    shader.locs[LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){ 0.2f, 0.2f, 0.2f, 1.0f }, UNIFORM_VEC4);


    Light light_1 = CreateLight(LIGHT_POINT, (Vector3){ -105, 5, -105 }, Vector3Zero(), RED, shader);
    Light light_2 = CreateLight(LIGHT_POINT, (Vector3){ -105, 5, 105 }, Vector3Zero(), BLUE, shader);
    Light light_3 = CreateLight(LIGHT_POINT, (Vector3){ 105, 5, -105 }, Vector3Zero(), PURPLE, shader);
    UpdateLightValues(shader, light_1);
    UpdateLightValues(shader, light_2);
    UpdateLightValues(shader, light_3);

    SetCameraMode(camera, CAMERA_CUSTOM);

    car = LoadModel("assets/car.obj");
    car.materials[0].shader = shader;

    camera.fovy = 45.0f;
    camera.target = (Vector3){.0f, .0f, .0f};
    camera.position = (Vector3){0.0f, 10.0f, 30.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.type = CAMERA_PERSPECTIVE;

    for(int i = 0; i < PARTICLE_COUNT; i++){
        particles[i].position.x += 0.1f;
        particles[i].position.z += 0.1f;
    }

#ifdef OS_WEB
    emscripten_set_main_loop(update_frame, 0, 1);
#else
    while (!WindowShouldClose())
    {
        update_frame();
    }
#endif
    CloseWindow();

    UnloadTexture(smoke_texture);
    UnloadModel(car);

    return 0;
}