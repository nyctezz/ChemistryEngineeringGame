#include "window.h"


bool window_init(_window* window, const char* title, int width, int height)
{
    window->handle = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL);

    if (window->handle != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void window_destroy(_window* window)
{
    if (window->handle)
    {
        SDL_DestroyWindow(window->handle);
    }  
}