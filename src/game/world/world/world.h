#pragma once

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../tile/tile.h"

typedef struct {

    int width; //in number of tiles
    int height;

    uint32_t seed;

    //_tile* tiles;
    _tile_stack* stacks; // array of tile stacks, (all tiles sum up to world size)

} _world;

_tile* world_get_tile(_world* world, int x, int y);
void world_init(_world* world, int radius, uint32_t seed);
void world_generate(_world* world, int radius);
void world_generate_flat(_world* world, int x, int y);
void world_destroy(_world* world);