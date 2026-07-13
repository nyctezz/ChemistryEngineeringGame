#pragma once

#include <glad/glad.h>
#include <SDL3/SDL.h>

#include "window.h"
#include "../graphics/shader.h"


typedef struct
{
    SDL_GLContext context;
    _shader shader;
} _renderer;

int renderer_init(_renderer* renderer, _window* window);
void renderer_run(_renderer* renderer);
void renderer_destroy(_renderer* renderer);