#ifndef PHYSICS_H
#define PHYSICS_H
#include <raymath.h>
#include "track.h"

typedef struct particle_t {
    // In order to calculate the position with the speed implicit, we need actual and last position
    Vector3 position;
    Vector3 position_last;
    track_cell_t track_cell;
} particle_t;

void particle_update(particle_t* particle, float drag);
void particle_restrict(particle_t* particle, float restriction_side);
void particle_fix_distance(particle_t* particle1, particle_t* particle2);
void particle_calculate_cel(particle_t* particle, track_cell_t track[MAP_SIZE_Z][MAP_SIZE_X]);
#endif

#ifdef PHYSICS_H_IMPLEMENTATION

void particle_fix_distance(particle_t* particle1, particle_t* particle2){
    Vector3 distance_vector = Vector3Subtract(particle2->position, particle1->position);
    distance_vector = Vector3Normalize(distance_vector);
    distance_vector = Vector3Scale(distance_vector, 3.0f);
    particle2->position = Vector3Add(particle1->position, distance_vector);
}

void particle_restrict(particle_t* particle, float restriction_side) {
    if(particle->track_cell.collision_flags & COLLISION_UP){
        particle->position.z = fmax(particle->position.z, particle->track_cell.bounds.y);
    } 

    if(particle->track_cell.collision_flags & COLLISION_LEFT){
        particle->position.x = fmax(particle->position.x, particle->track_cell.bounds.x);
    } 

    if(particle->track_cell.collision_flags & COLLISION_RIGHT){
        particle->position.x = fmin(particle->position.x, particle->track_cell.bounds.width);
    } 

    if(particle->track_cell.collision_flags & COLLISION_DOWN){
        particle->position.z = fmin(particle->position.z, particle->track_cell.bounds.height);
    }
}

void particle_calculate_cel(particle_t* particle, track_cell_t track[MAP_SIZE_Z][MAP_SIZE_X]){
    int i = (particle->position.z + MAP_ZOOM * 0.5) / MAP_ZOOM;
    int j = (particle->position.x + MAP_ZOOM * 0.5) / MAP_ZOOM;

    particle->track_cell = track[i][j];
}

void particle_update(particle_t* particle, float drag){
    Vector3 temp_position = particle->position;
    Vector3 difference = Vector3Scale(Vector3Subtract(particle->position, particle->position_last), drag);
    particle->position = Vector3Add(particle->position, difference);
    particle->position_last = temp_position;
}
#undef PHYSICS_H_IMPLEMENTATION
#endif