#include "renderer.h"


int renderer_init(_renderer* renderer, _window* window)
{
    renderer->context = SDL_GL_CreateContext(window->handle);

    if (!renderer->context)
    {
        return 1;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        return 2;
    }

    SDL_GL_SetSwapInterval(1); //vsync on

    return 0;
}