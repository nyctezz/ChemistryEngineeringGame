#pragma once

#include <SDL3/SDL.h>

typedef struct
{
    float x;
    float y;

    float speed;
    float fast_speed;
    float zoom;
} _camera;

void camera_init(_camera* camera, float starting_x, float starting_y, float speed, float fast_speed, float starting_zoom);
void camera_handle_event(_camera* camera, SDL_Event* event);
void camera_update(_camera* camera, float delta_time);