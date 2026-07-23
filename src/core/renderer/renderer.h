#pragma once

#include <math.h>

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <cglm/cglm.h>

#include "../window/window.h"
#include "../mesh/mesh.h"
#include "../camera/camera.h"
#include "../../graphics/shader/shader.h"
#include "../../game/world/world/world.h"
#include "../../graphics/texture/texture.h"


typedef struct
{
    SDL_GLContext context;
    _shader shader;

    //world rendering:
    _mesh hex_mesh;

    //TODO: enum (from tile.h) and array this
    GLuint scapolite_texture;
    GLuint water_texture;
    GLuint rock_texture;

} _renderer;

int renderer_init(_renderer* renderer, _window* window);
void update_viewport(_window* window);
void renderer_draw_world_tile(_renderer* renderer, _tile* tile, int tile_x, int tile_y);
void renderer_run(_renderer* renderer, _world* world, _camera* camera, _window* window);
void renderer_destroy(_renderer* renderer);