#include "camera.h"

#include <math.h>

void camera_init(_camera* camera)
{
    camera->x = 0.0f;
    camera->y = 0.0f;

    camera->speed = 5.0f;
    camera->zoom = 4.0f;
}

void camera_handle_event(_camera* camera, SDL_Event* event)
{
    // Currently no event-based input is necessary.
    //
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


    if (keyboard[SDL_SCANCODE_Q])
    {
        camera->zoom = fmaxf(0.1f, camera->zoom - movement);
    }

    if (keyboard[SDL_SCANCODE_E])
    {
        camera->zoom += movement;
    }
}