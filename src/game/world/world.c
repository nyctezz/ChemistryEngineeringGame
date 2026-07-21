#include "world.h"


_tile* world_get_tile(_world* world, int x, int y)
{
    return &world->tiles[y * world->width + x];
}

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

void world_destroy(_world* world)
{
    free(world->tiles);
    world->tiles = NULL;
}