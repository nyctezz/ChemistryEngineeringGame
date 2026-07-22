#pragma once

typedef enum {
    TILE_NONE,
    TILE_SCRAPOLITE,
    TILE_WATER,
    TILE_DEEP_WATER,
    TILE_ROCK
} _tile_type;


typedef struct {

    _tile_type type;
    _tile_type type_under; //used only for water (what tile is under? for shallow water rendering and if water drains)

} _tile;