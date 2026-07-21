#include "renderer.h"


void update_viewport(_window* window)
{
    int width = window->width;
    int height = window->height;

    int viewport_size;

    if (width < height)
    {
        viewport_size = width;
    }
    else
    {
        viewport_size = height;
    }

    int x_offset = (width - viewport_size) / 2;
    int y_offset = (height - viewport_size) / 2;

    glViewport(x_offset, y_offset, viewport_size, viewport_size);
}


int renderer_init(_renderer* renderer, _window* window)
{
    /* RETURNED ERROR CODES:
    0 - success
    1 - failed to create OpenGL context
    2 - failed to initialize GLAD
    3 - renderer error
    */

    renderer->context = SDL_GL_CreateContext(window->handle);

    if (!renderer->context)
    {
        return 1;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        return 2;
    }

    if (shader_init(&renderer->shader, "assets/shaders/default.vert", "assets/shaders/default.frag") != 0)
    {
        return 3;
    }

    shader_use(&renderer->shader);
    shader_set_int(&renderer->shader, "ourTexture", 0);
    
    // set rendering options:
    SDL_GL_SetSwapInterval(1); //vsync on

    // set up viewport on window properly:
    update_viewport(window);

    // enable alpha blending for transparency:
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    //TODO: write correct error handling for this:

    mesh_init_hex(&renderer->hex_mesh);

    renderer->grass_texture = texture_load_png("assets/textures/debug.png");
    renderer->water_texture = texture_load_png("assets/textures/debug.png");
    renderer->rock_texture  = texture_load_png("assets/textures/debug.png");

    return 0;
}

void renderer_draw_tile(_renderer* renderer, _camera* camera, _tile* tile, int tile_x, int tile_y)
{
    float world_x;
    float world_y;

    hex_to_world(tile_x, tile_y, &world_x, &world_y);

    // Apply camera
    world_x -= camera->x;
    world_y -= camera->y;

    glActiveTexture(GL_TEXTURE0);
    switch(tile->type)
    {
        case TILE_GRASS:
            glBindTexture(GL_TEXTURE_2D, renderer->grass_texture);
            break;

        case TILE_WATER:
            glBindTexture(GL_TEXTURE_2D, renderer->water_texture);
            break;

        case TILE_ROCK:
            glBindTexture(GL_TEXTURE_2D, renderer->rock_texture);
            break;
    }

    shader_set_vec2( &renderer->shader, "uTranslation", world_x, world_y);

    mesh_draw(&renderer->hex_mesh);
}

#define HEX_RADIUS 1.0f

void hex_to_world(int tile_x, int tile_y, float* world_x, float* world_y)
{
    *world_x = HEX_RADIUS * 1.5f * tile_x;
    *world_y = HEX_RADIUS * (sqrtf(3.0f) * (0.5f * tile_x - tile_y));
}

void renderer_run(_renderer* renderer, _world* world, _camera* camera)
{
    shader_use(&renderer->shader);

    for (int y = 0; y < world->height; y++)
    {
        for (int x = 0; x < world->width; x++)
        {
            _tile* tile = world_get_tile(world, x, y);

            renderer_draw_tile(renderer, camera, tile, x, y);
        }
    }
}

void renderer_destroy(_renderer* renderer)
{
    mesh_destroy(&renderer->hex_mesh);

    glDeleteTextures(1, &renderer->grass_texture);
    glDeleteTextures(1, &renderer->water_texture);
    glDeleteTextures(1, &renderer->rock_texture);

    shader_destroy(&renderer->shader);
}