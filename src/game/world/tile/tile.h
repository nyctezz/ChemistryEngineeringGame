#pragma once

#include <stddef.h>

typedef enum {
    TILE_NONE, //has to be first
    TILE_SCAPOLITE, //has to be second

    TILE_WATER,
    TILE_DEEP_WATER,
    TILE_ROCK,

    COUNT_TILE_TYPES //has to be last
} _tile_type;


/*
typedef struct {

    _tile_type type;
    _tile_type type_under; //used only for water (what tile is under? for shallow water rendering and if water drains)

} _tile;
*/

typedef struct {
    int x;
    int y;

    _tile_type type_under; // so far used only for water (what tile is under? for shallow water double rendering and if water drains)
} _tile;

typedef struct {
    _tile* tiles; // resizeable array of tiles
    size_t count; // current number of tiles
    size_t capacity; // max count avaliable before needing realloc to increase it

    //_tile_type type; // type of tiles held in this stack SET BY INDEX OF THE STACK
} _tile_stack;
