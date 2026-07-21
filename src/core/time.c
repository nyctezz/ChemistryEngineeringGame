#include "time.h"

#include <SDL3/SDL.h>

void timer_init(_timer* timer)
{
    timer->previous_time = SDL_GetTicks();
}

float timer_get_delta_time(_timer* timer)
{
    uint64_t current_time = SDL_GetTicksNS();

    float delta_time =
        (float)(current_time - timer->previous_time) / 1000000000.0f;

    timer->previous_time = current_time;

    return delta_time;
}