#pragma once

#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>

#include "window.h"
#include "camera.h"
#include "renderer.h"
#include "../graphics/shader.h"
#include "../graphics/texture.h"
//#include "../game/gameobject.h"
#include "mesh.h" //comment it out later (move to gameobject)
#include "../game/world/world.h"


typedef struct {
    _window window;
    _renderer renderer;
    bool is_running;
    SDL_Event event;
} _application;


void application_init(_application* app);

void application_run(_application* app);
    
void application_destroy(_application* app);