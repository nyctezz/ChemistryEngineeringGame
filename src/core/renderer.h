#pragma once

#include <math.h>

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <cglm/cglm.h>

#include "window.h"
#include "mesh.h"
#include "camera.h"
#include "../graphics/shader.h"
#include "../game/world/world.h"
#include "../graphics/texture.h"


typedef struct
{
    SDL_GLContext context;
    _shader shader;

    //world rendering:
    _mesh hex_mesh;

    GLuint grass_texture;
    GLuint water_texture;
    GLuint rock_texture;

} _renderer;

int renderer_init(_renderer* renderer, _window* window);
void update_viewport(_window* window);
void renderer_draw_world_tile(_renderer* renderer, _tile* tile, int tile_x, int tile_y);
void renderer_run(_renderer* renderer, _world* world, _camera* camera, _window* window);
void renderer_destroy(_renderer* renderer);