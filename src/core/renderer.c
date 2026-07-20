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
    

    // set rendering options:
    SDL_GL_SetSwapInterval(1); //vsync on


    // set up viewport on window properly:
    update_viewport(window);

    // enable alpha blending for transparency:
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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