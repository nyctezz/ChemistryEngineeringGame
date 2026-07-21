#include "renderer.h"

//TODO: cleanup (minly math functions to math.c)

static void mat4_ortho(float* m, float left, float right, float bottom, float top, float near, float far)
{
    float rl = right - left;
    float tb = top - bottom;
    float fn = far - near;

    m[0] = 2.0f / rl;
    m[1] = 0.0f;
    m[2] = 0.0f;
    m[3] = 0.0f;

    m[4] = 0.0f;
    m[5] = 2.0f / tb;
    m[6] = 0.0f;
    m[7] = 0.0f;

    m[8] = 0.0f;
    m[9] = 0.0f;
    m[10] = -2.0f / fn;
    m[11] = 0.0f;

    m[12] = -(right + left) / rl;
    m[13] = -(top + bottom) / tb;
    m[14] = -(far + near) / fn;
    m[15] = 1.0f;
}

static void mat4_translate(float* m, float x, float y, float z)
{
    m[0] = 1.0f;
    m[1] = 0.0f;
    m[2] = 0.0f;
    m[3] = 0.0f;

    m[4] = 0.0f;
    m[5] = 1.0f;
    m[6] = 0.0f;
    m[7] = 0.0f;

    m[8] = 0.0f;
    m[9] = 0.0f;
    m[10] = 1.0f;
    m[11] = 0.0f;

    m[12] = x;
    m[13] = y;
    m[14] = z;
    m[15] = 1.0f;
}


void update_viewport(_window* window)
{
    glViewport(0, 0, window->width, window->height);
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

    shader_set_vec2(&renderer->shader, "uTranslation", world_x, world_y);

    mesh_draw(&renderer->hex_mesh);
}

#define HEX_RADIUS 1.0f

void hex_to_world(int tile_x, int tile_y, float* world_x, float* world_y)
{
    *world_x = HEX_RADIUS * 1.5f * tile_x;
    *world_y = HEX_RADIUS * (sqrtf(3.0f) * (0.5f * tile_x - tile_y));
}

void renderer_run(_renderer* renderer, _world* world, _camera* camera, _window* window)
{
    //set background color and clear the screen:
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    shader_use(&renderer->shader);

    float aspect = (float)window->width / (float)window->height;
    float half_height = camera->zoom;
    float half_width = half_height * aspect;

    float projection[16];
    mat4_ortho(projection, -half_width, half_width, -half_height, half_height, -1.0f, 1.0f);

    float view[16];
    mat4_translate(view, -camera->x, -camera->y, 0.0f);

    shader_set_mat4(&renderer->shader, "uProjection", projection);
    shader_set_mat4(&renderer->shader, "uView", view);

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