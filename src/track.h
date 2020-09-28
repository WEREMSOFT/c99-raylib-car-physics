#ifndef TRACK_H
#define TRACK_H

#include <stdint.h>
#include <stdbool.h>

#define MAP_SIZE_X 7
#define MAP_SIZE_Z 4
#define MAP_ZOOM 5

#define COLLISION_LEFT 0x01
#define COLLISION_RIGHT 0x02
#define COLLISION_UP 0x04
#define COLLISION_DOWN 0x08

typedef struct track_cell_t {
    bool enabled;
    uint8_t collision_flags;
} track_cell_t;

void track_init(track_cell_t track[MAP_SIZE_Z][MAP_SIZE_X]);

#endif

#ifdef TRACK_H_IMPLEMENTATION

void track_init(track_cell_t track[MAP_SIZE_Z][MAP_SIZE_X]){
        char map[MAP_SIZE_Z][MAP_SIZE_X] = {
                        {1, 1, 1, 1, 1, 1, 1},
                        {1, 0, 0, 0, 0, 0, 1},
                        {1, 0, 1, 1, 1, 0, 1},
                        {1, 1, 1, 0, 1, 1, 1},
                        };

    for(int i = 0; i < MAP_SIZE_Z; i++){
        for(int j = 0; j < MAP_SIZE_X; j++){
            if(track[i][j].enabled = map[i][j]){
                if(i == 0) track[i][j].collision_flags |= COLLISION_UP;
                if(j == 0) track[i][j].collision_flags |= COLLISION_LEFT;
                if(i == MAP_SIZE_Z-1) track[i][j].collision_flags |= COLLISION_DOWN;
                if(j == MAP_SIZE_X-1) track[i][j].collision_flags |= COLLISION_RIGHT;
            }

        }
    }
}

#undef TRACK_H_IMPLEMENTATION
#endif