#ifndef CAR_H
#define CAR_H

#include <raylib.h>
#include "track.h"

enum car_controls_e {
    CONTROL_UP,
    CONTROL_LEFT,
    CONTROL_RIGHT
};

typedef struct car_t {
    Model model;
    Color color;
    Vector3 acceleration;
    float angle;
    Quaternion direction;
    particle_t particle_head;
    particle_t particle_tail;
    unsigned int controls[3];
} car_t;

void car_init(car_t* car, Model model, Color color, unsigned int controls[3]);
void car_update(car_t* car, track_cell_t track[MAP_SIZE_Z][MAP_SIZE_X]);
void car_draw(car_t* car);
#endif

#ifdef CAR_H_IMPLEMENTATION
#undef CAR_H_IMPLEMENTATION

void car_init(car_t* car, Model model, Color color, unsigned int controls[3]){
    car->model = model;
    car->color = color;

    car->controls[CONTROL_UP] = controls[CONTROL_UP];
    car->controls[CONTROL_LEFT] = controls[CONTROL_LEFT];
    car->controls[CONTROL_RIGHT] = controls[CONTROL_RIGHT];

    car->particle_tail.position.x += 0.1f;
    car->particle_tail.position.z += 0.1f;
}

void car_update(car_t* car, track_cell_t track[MAP_SIZE_Z][MAP_SIZE_X]){

    car->acceleration = Vector3Zero();

    car->acceleration.z += 0.04f * IsKeyDown(car->controls[CONTROL_UP]);
    car->angle += 0.05f * IsKeyDown(car->controls[CONTROL_LEFT]);
    car->angle -= 0.05f * IsKeyDown(car->controls[CONTROL_RIGHT]);
    
    car->direction = QuaternionFromAxisAngle((Vector3){0, 1.0f, 0}, car->angle);

    car->acceleration = Vector3RotateByQuaternion(car->acceleration, car->direction);

    car->particle_head.position = Vector3Add(car->particle_head.position, car->acceleration);

    particle_update(&car->particle_head, 0.95);
    particle_update(&car->particle_tail, 0.96);

    particle_fix_distance(&car->particle_head, &car->particle_tail);

    // particle_restrict(&car->particle_head, 48.5f, track);
    // particle_restrict(&car->particle_tail, 48.5f, track);
}

void car_draw(car_t* car){
    car->model.transform = MatrixRotateY(Vector2Angle((Vector2){car->particle_tail.position.x, car->particle_tail.position.z}, (Vector2){car->particle_head.position.x, car->particle_head.position.z}) / RAD2DEG);
    DrawModel(car->model, car->particle_head.position, 0.5f, car->color);
}

#endif