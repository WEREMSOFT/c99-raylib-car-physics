#ifndef PHYSICS_H
#define PHYSICS_H
#include <raymath.h>

typedef struct particle_t {
    Vector3 position;
    Vector3 position_last;
    float mass_inverse;
} particle_t;

void particle_update(particle_t* particle, float drag);
void particle_restrict(particle_t* particle, float restriction_side);
void particle_fix_distance(particle_t* particle1, particle_t* particle2);
#endif

#ifdef __INCLUDED_IN_MAIN__

void particle_fix_distance(particle_t* particle1, particle_t* particle2){
    Vector3 distance_vector = Vector3Subtract(particle2->position, particle1->position);
    distance_vector = Vector3Normalize(distance_vector);
    distance_vector = Vector3Scale(distance_vector, 3.0f);
    particle2->position = Vector3Add(particle1->position, distance_vector);
}

void particle_restrict(particle_t* particle, float restriction_side) {
    particle->position = Vector3Min((Vector3){restriction_side, 0.0, restriction_side}, Vector3Max((Vector3){-restriction_side, 0.0, -restriction_side}, particle->position));
    particle->position_last = Vector3Min((Vector3){restriction_side, 0.0, restriction_side}, Vector3Max((Vector3){-restriction_side, 0.0, -restriction_side}, particle->position_last));
}

void particle_update(particle_t* particle, float drag){
    Vector3 temp_position = particle->position;
    Vector3 difference = Vector3Scale(Vector3Subtract(particle->position, particle->position_last), drag);
    particle->position = Vector3Add(particle->position, difference);
    particle->position_last = temp_position;
}

#endif