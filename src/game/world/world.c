#include "world.h"

// local helper functions:
static void hex_to_world(int tile_x, int tile_y, float* world_x, float* world_y)
{
    *world_x = 1.5f * tile_x;
    *world_y = sqrtf(3.0f) * (tile_y + 0.5f * tile_x);
}
// ---


_tile* world_get_tile(_world* world, int x, int y)
{
    return &world->tiles[y * world->width + x];
}

/*
void world_init(_world* world, int width, int height)
{
    world->width = width;
    world->height = height;

    world->tiles = malloc(sizeof(_tile) * width * height);

    if (world->tiles == NULL)
    {
        printf("Failed to allocate memory for world.\n");
        abort();
    }
}

void world_generate(_world* world) //later make this generate procedurally
{
    for (int y = 0; y < world->height; y++)
    {
        for (int x = 0; x < world->width; x++)
        {
            int index = y * world->width + x;

            if (y == 0 || y == world->height - 1 || x == 0 || x == world->width - 1)
            {
                world->tiles[index].type = TILE_ROCK;
            }
            else
            {
                world->tiles[index].type = TILE_GRASS;
            }
        }
    }
}
*/

void world_init(_world* world, int radius)
{
    world->width = radius * 2 + 1;
    world->height = radius * 2 + 1;

    world->tiles = malloc(sizeof(_tile) * world->width * world->height);

    if (world->tiles == NULL)
    {
        printf("Failed to allocate memory for world.\n");
        abort();
    }

    world_generate(world, radius);
}

void world_generate(_world* world, int radius)
{
    for (int y = 0; y < world->height; y++)
    {
        for (int x = 0; x < world->width; x++)
        {
            int centered_x = x - radius;
            int centered_y = y - radius;

            float world_x;
            float world_y;

            hex_to_world(centered_x, centered_y, &world_x, &world_y);

            float distance = sqrtf(world_x * world_x + world_y * world_y);

            _tile* tile = world_get_tile(world, x, y);

            if (distance > radius * 1.5f)
            {
                tile->type = TILE_NONE;
            }
            else if (distance > radius * 1.2f)
            {
                tile->type = TILE_ROCK;
            }
            else
            {
                tile->type = TILE_GRASS;
            }
        }
    }
}

void world_destroy(_world* world)
{
    free(world->tiles);
    world->tiles = NULL;
}