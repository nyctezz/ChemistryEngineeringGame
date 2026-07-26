#pragma once

#include <stdint.h>
#include <stdio.h>

typedef struct
{
    uint64_t previous_time;
    float delta_time;

    //used for stable, average fps calculation:
    uint32_t frame_counter;
    float average_timer; // keeps track of time intervals, returns to 0 if certain amount of time elapsed
} _timer;

void timer_init(_timer* timer);
void timer_update(_timer* timer);
void timer_print_fps(_timer* timer);