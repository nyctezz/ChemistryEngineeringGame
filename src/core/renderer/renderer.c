#include "renderer.h"


// local helper functions:
static void hex_to_world(int tile_x, int tile_y, float* world_x, float* world_y)
{
    *world_x = 1.5f * tile_x;
    *world_y = 1.7320508f * (tile_y + 0.5f * tile_x);
}

static GLuint renderer_get_tile_texture(const _renderer* renderer, _tile_type type)
{
    switch (type)
    {
        case TILE_NONE:
            return 0;

        case TILE_SCAPOLITE:
            return renderer->scapolite_texture;

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
    SDL_GL_SetSwapInterval(0); //vsync on

    // set up viewport on window properly:
    update_viewport(window);

    // enable alpha blending for transparency:
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // initialize assets:

    //TODO: write proper error handling for this:
    mesh_init_hex(&renderer->hex_mesh);

    renderer->scapolite_texture = texture_load_png("assets/textures/scapolite.png");
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

    glDrawElements(GL_TRIANGLES, renderer->hex_mesh.index_count, GL_UNSIGNED_INT, NULL);
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

    glActiveTexture(GL_TEXTURE0);

    // alpha blending
    glEnable(GL_BLEND); // TODO: check if I'm not doing those two settings when initializing 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_set_vec4(&renderer->shader, "uColorTint", 1.0f, 1.0f, 1.0f, 1.0f);

    // Render opaque terrain
    for (int type = TILE_SCAPOLITE; type < COUNT_TILE_TYPES; type++) //in enum type1 is 0, type2 is 1, do until you hit COUNT_TILE_TYPES (which is last), start on first tile
    {
        if (type == TILE_WATER) //skip water
        {
            continue;
        }

        _tile_stack* stack = &world->stacks[type];

        if (stack->count == 0) //skip if empty
        {
            continue;
        }

        GLuint texture = renderer_get_tile_texture(renderer, type);

        if (texture == 0) //skip
        {
            continue;
        }

        //bind texture and hex VAO outside the loop
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(renderer->hex_mesh.vao);

        for (size_t i = 0; i < stack->count; i++) // draw batch of the same object
        {
            _tile* tile = &stack->tiles[i];

            int centered_x = tile->x - world->width / 2;
            int centered_y = tile->y - world->height / 2;

            renderer_draw_world_tile(renderer, tile, centered_x, centered_y);
        }

        glBindVertexArray(0); //unbind VAO at the end of the batch
    }


    // render water
    _tile_stack* water_stack = &world->stacks[TILE_WATER];

    if (water_stack->count > 0) // skip if no water
    {
        GLuint water_texture = renderer_get_tile_texture(renderer, TILE_WATER);

        if (water_texture != 0)
        {
            glBindVertexArray(renderer->hex_mesh.vao); //bind VAO outside the loop

            for (size_t i = 0; i < water_stack->count; i++)
            {
                _tile* tile = &water_stack->tiles[i];

                int centered_x = tile->x - world->width / 2;
                int centered_y = tile->y - world->height / 2;

                // render the tile underneath the water
                GLuint under_texture = renderer_get_tile_texture(renderer, tile->type_under);

                if (under_texture != 0)
                {
                    glBindTexture(GL_TEXTURE_2D, under_texture);

                    shader_set_vec4(&renderer->shader, "uColorTint", 1.0f, 1.0f, 1.0f, 1.0f);

                    renderer_draw_world_tile(renderer, tile, centered_x, centered_y);
                }


                // Render transparent water on top.
                glBindTexture(GL_TEXTURE_2D, water_texture);

                shader_set_vec4(&renderer->shader, "uColorTint", 1.0f, 1.0f, 1.0f, 0.6f);

                renderer_draw_world_tile(renderer, tile, centered_x, centered_y);
            }

            glBindVertexArray(0); //unbind VAO at the end of the batch
        }
    }

    shader_set_vec4(&renderer->shader, "uColorTint", 1.0f, 1.0f, 1.0f, 1.0f);
}

void renderer_destroy(_renderer* renderer)
{
    mesh_destroy(&renderer->hex_mesh);

    glDeleteTextures(1, &renderer->scapolite_texture);
    glDeleteTextures(1, &renderer->water_texture);
    glDeleteTextures(1, &renderer->rock_texture);

    shader_destroy(&renderer->shader);
}