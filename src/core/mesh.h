#pragma once

#include <stdint.h>

#include <glad/glad.h>

typedef struct
{
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    float* vertices;
    uint32_t* indices;

    size_t vertex_count;
    size_t index_count;
} _mesh;

void mesh_init(_mesh* mesh, float* vertices, size_t vertex_count, uint32_t* indices, size_t index_count);
void mesh_init_worldtile(_mesh* mesh);
void mesh_draw(const _mesh* mesh);
void mesh_destroy(_mesh* mesh);