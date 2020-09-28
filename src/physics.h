#ifndef PHYSICS_H
#define PHYSICS_H
#include <raymath.h>
#include "track.h"

typedef struct particle_t {
    Vector3 position;
    Vector3 position_last;
    float mass_inverse;
} particle_t;

void particle_update(particle_t* particle, float drag);
void particle_restrict(particle_t* particle, float restriction_side, track_cell_t track[MAP_SIZE_Z][MAP_SIZE_X]);
void particle_fix_distance(particle_t* particle1, particle_t* particle2);
#endif

#ifdef PHYSICS_H_IMPLEMENTATION

void particle_fix_distance(particle_t* particle1, particle_t* particle2){
    Vector3 distance_vector = Vector3Subtract(particle2->position, particle1->position);
    distance_vector = Vector3Normalize(distance_vector);
    distance_vector = Vector3Scale(distance_vector, 3.0f);
    particle2->position = Vector3Add(particle1->position, distance_vector);
}

void particle_restrict(particle_t* particle, float restriction_side, track_cell_t track[MAP_SIZE_Z][MAP_SIZE_X]) {
    int i = particle->position.x / MAP_ZOOM;
    int j = particle->position.z / MAP_ZOOM;

    track_cell_t cell = track[i][j];

    if(cell.collision_flags && COLLISION_UP){
        particle->position.x = fmax(particle->position.x, 0 - MAP_ZOOM / 2);
    } 

    if(cell.collision_flags && COLLISION_RIGHT){
        particle->position.z = fmin(particle->position.z, MAP_SIZE_Z * MAP_ZOOM + MAP_ZOOM / 2);
    } 

    // particle->position = Vector3Min((Vector3){restriction_side, 0.0, restriction_side}, Vector3Max((Vector3){-restriction_side, 0.0, -restriction_side}, particle->position));
    // particle->position_last = Vector3Min((Vector3){restriction_side, 0.0, restriction_side}, Vector3Max((Vector3){-restriction_side, 0.0, -restriction_side}, particle->position_last));
}

void particle_update(particle_t* particle, float drag){
    Vector3 temp_position = particle->position;
    Vector3 difference = Vector3Scale(Vector3Subtract(particle->position, particle->position_last), drag);
    particle->position = Vector3Add(particle->position, difference);
    particle->position_last = temp_position;
}
#undef PHYSICS_H_IMPLEMENTATION
#endif