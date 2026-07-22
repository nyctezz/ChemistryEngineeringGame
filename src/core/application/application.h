#pragma once

#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>

#include "../window/window.h"
#include "../camera/camera.h"
#include "../time/time.h"
#include "../renderer/renderer.h"
#include "../../graphics/shader/shader.h"
#include "../../graphics/texture/texture.h"
//#include "../game/gameobject.h"
#include "../mesh/mesh.h" //comment it out later (move to gameobject)
#include "../../game/world/world/world.h"


typedef struct {
    SDL_Event event;
    _window window;
    _renderer renderer;
    _timer timer;
    bool is_running;

} _application;


void application_init(_application* app);
void application_run(_application* app);
void application_process_events(_application* app);
void application_destroy(_application* app);