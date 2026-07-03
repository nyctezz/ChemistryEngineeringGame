#include "renderer.h"


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
    

    // set rendering options:
    SDL_GL_SetSwapInterval(1); //vsync on

    return 0;
}

void renderer_run(_renderer* renderer)
{
    shader_use(&renderer->shader);
}

void renderer_destroy(_renderer* renderer)
{
    shader_destroy(&renderer->shader);
}