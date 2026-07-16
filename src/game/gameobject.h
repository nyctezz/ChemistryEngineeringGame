#pragma once

#include "../core/mesh.h"

typedef struct
{
    _mesh* mesh;
    //add texture later
} _gameobject;

void gameobject_init();
void gameobject_draw(_gameobject* gameobject);