#ifndef TRACK_H
#define TRACK_H

#include <stdint.h>
#include <stdbool.h>

#define MAP_SIZE_X 7
#define MAP_SIZE_Z 4
#define MAP_ZOOM 20

#define COLLISION_LEFT 0x01
#define COLLISION_RIGHT 0x02
#define COLLISION_UP 0x04
#define COLLISION_DOWN 0x08

typedef struct track_cell_t {
    uint8_t enabled;
    uint8_t collision_flags;
    Rectangle bounds;
} track_cell_t;

void track_init(track_cell_t track[MAP_SIZE_Z][MAP_SIZE_X]);

#endif

#ifdef TRACK_H_IMPLEMENTATION

void track_init(track_cell_t track[MAP_SIZE_Z][MAP_SIZE_X]){

    /* This is an array used to create the track. Collisions are calculated based on it.
    * The constants MAP_SIZE_Z and MAP_SIZE_X are self explanatory. You need to modify them
    * if you want a track of differnet characteristics.
    * The constant MAP_ZOOM is how big are the tiles.
    */

    uint8_t map[MAP_SIZE_Z][MAP_SIZE_X] = {
                    {1, 1, 1, 1, 1, 1, 1},
                    {1, 0, 0, 0, 0, 0, 1},
                    {1, 0, 1, 1, 1, 0, 1},
                    {1, 1, 1, 0, 1, 1, 1},
                    };
    /* 
    * This loop create an array of structs based on the map layout. The map layout is discarded after
    * the precalculation
    */
    for(int i = 0; i < MAP_SIZE_Z; i++){
        for(int j = 0; j < MAP_SIZE_X; j++){
            if((track[i][j].enabled = map[i][j])){

                track[i][j].bounds.x = MAP_ZOOM * (j - 0.49f);
                track[i][j].bounds.y = MAP_ZOOM * (i - 0.49f);
                track[i][j].bounds.width = MAP_ZOOM * (j + 0.49f);
                track[i][j].bounds.height = MAP_ZOOM * (i + 0.49f);


                if(i == 0 || !map[i - 1][j])
                    track[i][j].collision_flags |= COLLISION_UP;

                if(j == 0 || !map[i][j - 1])
                    track[i][j].collision_flags |= COLLISION_LEFT;
                
                if(i == MAP_SIZE_Z-1 || !map[i + 1][j])
                    track[i][j].collision_flags |= COLLISION_DOWN;


                if(j == MAP_SIZE_X-1 || !map[i][j + 1])
                    track[i][j].collision_flags |= COLLISION_RIGHT;

            }

        }
    }
}

#undef TRACK_H_IMPLEMENTATION
#endif