#include "renderer.h"


// local helper functions:
static void hex_to_world(int tile_x, int tile_y, float* world_x, float* world_y)
{
    *world_x = 1.5f * tile_x;
    *world_y = sqrtf(3.0f) * (tile_y + 0.5f * tile_x);
}

static GLuint renderer_get_tile_texture(const _renderer* renderer, _tile_type type)
{
    switch (type)
    {
        case TILE_NONE:
            return 0;

        case TILE_SCRAPOLITE:
            return renderer->scrapolite_texture;

        case TILE_WATER:
            return renderer->water_texture;
        
        case TILE_DEEP_WATER:
            return renderer->water_texture;

        case TILE_ROCK:
            return renderer->rock_texture;

        default:
            return 0;
    }
}
//---

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


    // initialize assets:

    //TODO: write proper error handling for this:
    mesh_init_hex(&renderer->hex_mesh);

    renderer->scrapolite_texture = texture_load_png("assets/textures/scapolite.png");
    renderer->water_texture = texture_load_png("assets/textures/water.png");
    renderer->rock_texture  = texture_load_png("assets/textures/rock.png");

    return 0;
}

void update_viewport(_window* window)
{
    glViewport(0, 0, window->width, window->height);
}

void renderer_draw_world_tile(_renderer* renderer, _tile* tile, int tile_x, int tile_y)
{
    float world_x;
    float world_y;

    hex_to_world(tile_x, tile_y, &world_x, &world_y);
    shader_set_vec2(&renderer->shader, "uTranslation", world_x, world_y);

    mesh_draw(&renderer->hex_mesh);
}

void renderer_run(_renderer* renderer, _world* world, _camera* camera, _window* window)
{
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader_use(&renderer->shader);

    float aspect = (float)window->width / (float)window->height;

    float half_height = camera->zoom;
    float half_width = half_height * aspect;

    mat4 projection;
    glm_ortho(-half_width, half_width, -half_height, half_height, -1.0f, 1.0f, projection);

    mat4 view;
    glm_mat4_identity(view);

    vec3 camera_position = {-camera->x, -camera->y, 0.0f};
    glm_translate(view, camera_position);

    shader_set_mat4(&renderer->shader, "uProjection", projection);
    shader_set_mat4(&renderer->shader, "uView", view);


    //alpha rendering
    shader_set_vec4(&renderer->shader, "uColorTint", 1.0f, 1.0f, 1.0f, 1.0f);


    GLuint current_texture = 0;

    glActiveTexture(GL_TEXTURE0);

    for (int y = 0; y < world->height; y++)
    {
        for (int x = 0; x < world->width; x++)
        {
            const _tile* tile = world_get_tile(world, x, y);

            if (tile->type == TILE_NONE)
            {
                continue;
            }

            GLuint texture = renderer_get_tile_texture(renderer, tile->type);

            if (texture == 0)
            {
                continue;
            }

            int centered_x = x - world->width / 2;
            int centered_y = y - world->height / 2;


            if (tile->type_under != TILE_NONE) //if any texture is transparent and has something underneath:
            {
                //=-= double rendering with transparent water: =-=

                // draw what's under water:
                GLuint under_texture = renderer_get_tile_texture(renderer, tile->type_under);

                glBindTexture(GL_TEXTURE_2D, under_texture);
                current_texture = under_texture;

                shader_set_vec4(&renderer->shader, "uColorTint", 1.0f, 1.0f, 1.0f, 1.0f);
                renderer_draw_world_tile(renderer, tile, centered_x, centered_y);



                // draw transparend water underneath:
                glBindTexture(GL_TEXTURE_2D, texture);
                current_texture = texture;

                shader_set_vec4(&renderer->shader, "uColorTint", 1.0f, 1.0f, 1.0f, 0.6f);
                renderer_draw_world_tile(renderer, tile, centered_x, centered_y);

                shader_set_vec4(&renderer->shader, "uColorTint", 1.0f, 1.0f, 1.0f, 1.0f);  // important to end with this set to 1.0f
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, texture);
                current_texture = texture;

                renderer_draw_world_tile(renderer, tile, centered_x, centered_y);
            }

            /*
            if (texture != current_texture)
            {
                glBindTexture(GL_TEXTURE_2D, texture);
                current_texture = texture;
            }
            

            renderer_draw_world_tile(renderer, tile, centered_x, centered_y);
            */
        }
    }
}

void renderer_destroy(_renderer* renderer)
{
    mesh_destroy(&renderer->hex_mesh);

    glDeleteTextures(1, &renderer->scrapolite_texture);
    glDeleteTextures(1, &renderer->water_texture);
    glDeleteTextures(1, &renderer->rock_texture);

    shader_destroy(&renderer->shader);
}