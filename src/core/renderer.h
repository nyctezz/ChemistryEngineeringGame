#pragma once

#include <GLAD/glad.h>
#include <SDL3/SDL.h>

#include "window.h"


typedef struct
{
    SDL_GLContext context;
} _renderer;

int renderer_init(_renderer* renderer, _window* window);