#include "camera.h"

#include <math.h>

void camera_init(_camera* camera, float starting_x, float starting_y, float speed, float fast_speed, float starting_zoom)
{
    camera->x = starting_x;
    camera->y = starting_y;

    camera->speed = speed;
    camera->fast_speed = fast_speed;
    camera->zoom = starting_zoom;
}

void camera_handle_event(_camera* camera, SDL_Event* event)
{
    // Currently no event-based input is necessary.
    // I will use SDL_GetKeyboardState() in camera_update()
}

void camera_update(_camera* camera, float delta_time)
{
    const Uint8* keyboard = (const Uint8*)SDL_GetKeyboardState(NULL);

    float movement = camera->speed * delta_time;

    if (keyboard[SDL_SCANCODE_W])
    {
        camera->y += movement;
    }

    if (keyboard[SDL_SCANCODE_S])
    {
        camera->y -= movement;
    }

    if (keyboard[SDL_SCANCODE_A])
    {
        camera->x -= movement;
    }

    if (keyboard[SDL_SCANCODE_D])
    {
        camera->x += movement;
    }


    if (keyboard[SDL_SCANCODE_LCTRL])
    {
        camera->zoom = fmaxf(0.1f, camera->zoom - movement);
    }

    if (keyboard[SDL_SCANCODE_SPACE])
    {
        camera->zoom += movement;
    }

    if (keyboard[SDL_SCANCODE_LSHIFT])
    {
        camera->speed = 10.0f;
    }
    else
    {
        camera->speed = 5.0f;
    }
}