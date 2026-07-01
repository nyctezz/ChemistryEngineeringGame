#include "window.h"


bool window_init(_window* window, const char* title, int width, int height, bool is_fullscreen)
{
    //OPENGL:
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


    //SDL window:
    window->handle = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_SetWindowFullscreen(window->handle, is_fullscreen);

    window->width = width;
    window->height = height;
    window->is_fullscreen = is_fullscreen;


    if (window->handle != NULL)
    {
        return true;
    }

    return false;
}

void window_destroy(_window* window)
{
    if (window->handle)
    {
        SDL_DestroyWindow(window->handle);
    }  
}