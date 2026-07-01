#pragma once

#include <SDL3/SDL.h>


typedef struct 
{
    SDL_Window* handle;

    int width;
    int height;
    bool is_fullscreen;
} _window;

bool window_init(_window* window, const char* title, int width, int height, bool is_fullscreen);

void window_destroy(_window* window);