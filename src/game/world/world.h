#pragma once

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "tile.h"

typedef struct {

    int width; //in number of tiles
    int height;

    _tile* tiles;

} _world;

_tile* world_get_tile(_world* world, int x, int y);
void world_init(_world* world, int radius);
void world_generate(_world* world, int radius);
void world_destroy(_world* world);