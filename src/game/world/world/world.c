#include "world.h"


// =-=-=-=-= procedural generation =-=-=-=-=

/*
> idea: 
create a hash functions with deterministic, pseudo-random output based on coordinates and seed

to get blobs of water, linearly interpolate between those random values to get a noise map, then treshold it to get binary map of where water should be
*/

static uint32_t hash_2d(int x, int y, uint32_t seed)
{
    uint32_t h = seed;

    h = h ^ ((uint32_t)x * 374761393u);
    h = h ^ ((uint32_t)y * 668265263u);

    h = (h ^ (h >> 13)) * 1274126177u;
    h = h ^ (h >> 16);

    return h;
}

static float hash_2d_float(int x, int y, uint32_t seed)
{
    return hash_2d(x, y, seed) / (float)UINT32_MAX;
}

static float lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

static float smoothstep(float t)
{
    return t * t * (3.0f - 2.0f * t);
}

static float fade(float t)
{
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

static float value_noise_2d(float x, float y, float scale, uint32_t seed)
{
    x /= scale;
    y /= scale;

    int x0 = (int)floorf(x);
    int y0 = (int)floorf(y);

    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float tx = x - x0;
    float ty = y - y0;

    //tx = smoothstep(tx);
    //ty = smoothstep(ty);

    tx = fade(tx);
    ty = fade(ty);

    float v00 = hash_2d_float(x0, y0, seed);
    float v10 = hash_2d_float(x1, y0, seed);
    float v01 = hash_2d_float(x0, y1, seed);
    float v11 = hash_2d_float(x1, y1, seed);

    float top = lerp(v00, v10, tx);
    float bottom = lerp(v01, v11, tx);

    return lerp(top, bottom, ty);
}
// =-=-=-=-= || =-=-=-=-= 


// local helper functions:
static void hex_to_world(int tile_x, int tile_y, float* world_x, float* world_y)
{
    *world_x = 1.5f * tile_x;
    *world_y = sqrtf(3.0f) * (tile_y + 0.5f * tile_x);
}

/*
_tile* world_get_tile(_world* world, int x, int y)
{
    return &world->tiles[y * world->width + x];
}
*/

static bool is_buffer_full(_tile_stack* stack)
{
    return stack->count >= stack->capacity;
}

static void buffer_expand(_tile_stack* stack, size_t how_much)
{
    stack->capacity += how_much;

    _tile* new_tiles = realloc(stack->tiles, sizeof(_tile) * stack->capacity);

    if (new_tiles == NULL)
    {
        printf("Failed to resize tile stack in function 'buffer_expand()' inside 'world.c'\n");
        abort();
    }

    stack->tiles = new_tiles;
}

static void push_tile_to_stack(_tile_stack* stack, int x, int y, _tile_type type_under)
{
    if (is_buffer_full(stack))
    {
        buffer_expand(stack, 64);
    }

    stack->tiles[stack->count].x = x;
    stack->tiles[stack->count].y = y;

    stack->tiles[stack->count].type_under = type_under;

    stack->count++;
}
// ---



// piecewise generation:

void world_generate_base(_world* world, int radius) // generates the base terrain (circle of rocks and grass inside)
{
    float rock_border_thickness = 2.0f; //in world units, not tiles

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

            //_tile* tile = world_get_tile(world, x, y);

            if (distance > radius)
            {
                //tile->type = TILE_NONE;
                push_tile_to_stack(&world->stacks[TILE_NONE], x, y, TILE_NONE);
            }
            else if (distance > (radius - rock_border_thickness))
            {
                //tile->type = TILE_ROCK;
                push_tile_to_stack(&world->stacks[TILE_ROCK], x, y, TILE_NONE);
            }
            else
            {
                //tile->type = TILE_SCAPOLITE;
                push_tile_to_stack(&world->stacks[TILE_SCAPOLITE], x, y, TILE_NONE);
            }
        }
    }
}

/*
void world_generate_water(_world* world, int radius) // generates water on tiles with grass (further extend by including deep and deep deep water, with higher thresholds for each)
{
    for (int y = 0; y < world->height; y++)
    {
        for (int x = 0; x < world->width; x++)
        {
            _tile* tile = world_get_tile(world, x, y);
            if (tile->type != TILE_SCAPOLITE) // generate water only on scapolite
            {
                continue;
            }

            int centered_x = x - radius;
            int centered_y = y - radius;

            float large_noise = value_noise_2d(centered_x, centered_y, 9.0f, world->seed); //third argument is general water size (noise map scale)
            float small_noise = value_noise_2d(centered_x, centered_y, 2.0f, world->seed + 1); //third argument is edge definition (higher value = more blob)
            float noise = large_noise * 0.8f + small_noise * 0.2f;

            if (noise > 0.65f) //noise treshold (higher value = less water)
            {
                tile->type = TILE_DEEP_WATER;
            }
            else if (noise > 0.5)
            {
                //put whatever tile it was as tile under water, then set water on top:
                tile->type_under = tile->type;
                tile->type = TILE_WATER;
            }
        }
    }
}
*/

void world_generate_water(_world* world, int radius)
{
    _tile_stack* scapolite_stack = &world->stacks[TILE_SCAPOLITE];
    _tile_stack* water_stack = &world->stacks[TILE_WATER];
    _tile_stack* deep_water_stack = &world->stacks[TILE_DEEP_WATER];

    size_t i = 0;

    while (i < scapolite_stack->count)
    {
        _tile* tile = &scapolite_stack->tiles[i];

        int centered_x = tile->x - radius;
        int centered_y = tile->y - radius;

        float large_noise = value_noise_2d(centered_x, centered_y, 9.0f, world->seed);
        float small_noise = value_noise_2d(centered_x, centered_y, 2.0f, world->seed + 1);

        float noise = large_noise * 0.8f + small_noise * 0.2f;

        if (noise > 0.65f)
        {
            push_tile_to_stack(deep_water_stack, tile->x, tile->y, TILE_SCAPOLITE);

            // remove tile from scapolite stack. (swap deleted elemnt with the last element and cut it out, reducing size)
            scapolite_stack->tiles[i] = scapolite_stack->tiles[scapolite_stack->count - 1];

            scapolite_stack->count--;
        }
        else if (noise > 0.5f)
        {
            push_tile_to_stack(water_stack, tile->x, tile->y, TILE_SCAPOLITE);

            // Remove tile from scapolite stack.
            scapolite_stack->tiles[i] = scapolite_stack->tiles[scapolite_stack->count - 1];

            scapolite_stack->count--;
        }
        else
        {
            i++;
        }
    }
}
// ---------------

void world_init(_world* world, int radius,uint32_t seed)
{
    world->seed = seed;

    world->width = radius * 2 + 1;
    world->height = radius * 2 + 1;

    //world->tiles = malloc(sizeof(_tile) * world->width * world->height);
    world->stacks = malloc(sizeof(_tile_stack) * COUNT_TILE_TYPES);

    for (int i = 0; i < COUNT_TILE_TYPES; i++)
    {
        world->stacks[i].count = 0; // currently there are 0 elements in this stack
        world->stacks[i].capacity = 64; // set buffer for 64 tiles, if run out, increase by another 64
        world->stacks[i].tiles = malloc(sizeof(_tile) * world->stacks[i].capacity);
    }

    if (world->stacks == NULL)
    {
        printf("Failed to allocate memory for world.\n");
        abort();
    }

    world_generate(world, radius);
}

void world_generate(_world* world, int radius)
{
    world_generate_base(world, radius);
    world_generate_water(world, radius);

    // Later:
    // world_generate_forests(world, radius);
    // world_generate_mountains(world, radius);
    // world_generate_deserts(world, radius);
}

/*
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
*/

void world_destroy(_world* world)
{
    for (int i = 0; i < COUNT_TILE_TYPES; i++)
    {
        free(world->stacks[i].tiles);
        world->stacks[i].tiles = NULL;
    }

    free(world->stacks);
    world->stacks = NULL;
}