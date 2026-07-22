#pragma once

#include <stdint.h>

typedef struct
{
    uint64_t previous_time;
    float delta_time;
} _timer;

void timer_init(_timer* timer);
float timer_get_delta_time(_timer* timer);