#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "tile.h"

typedef struct {

    int width; //number of tiles
    int height;

    _tile* tiles;

} _world;

_tile* world_get_tile(_world* world, int x, int y);
void world_init(_world* world, int width, int height);
void world_generate(_world* world);
void world_destroy(_world* world);