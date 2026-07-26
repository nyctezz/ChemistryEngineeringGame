#include "time.h"

#include <SDL3/SDL.h>

void timer_init(_timer* timer)
{
    timer->previous_time = SDL_GetTicks();
}

void timer_update(_timer* timer)
{
    uint64_t current_time = SDL_GetTicksNS();

    timer->delta_time = (float)(current_time - timer->previous_time) / 1000000000.0f;
    timer->previous_time = current_time;
}

void timer_print_fps(_timer* timer) //returns average fps number every 0.5 second. If time didn't elapsed yet, returns -1
{
    timer->average_timer += timer->delta_time;
    timer->frame_counter++;

    if (timer->average_timer >= 0.5f)
    {
        float average_frame_time = timer->average_timer / timer->frame_counter;

        printf("FPS: %.2f\n", 1.0f / average_frame_time);

        timer->average_timer = 0.0f;
        timer->frame_counter = 0;
    }
}