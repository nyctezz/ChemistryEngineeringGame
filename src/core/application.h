#pragma once

#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>

#include "window.h"


typedef struct {
    _window window;
} _application;


void application_init(_application* app);

void application_run(_application* app);
    
void application_destroy(_application* app);