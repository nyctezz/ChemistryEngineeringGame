#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

typedef struct {
    unsigned int vertex_shader;
    unsigned int fragment_shader;
    unsigned int shader_program;
} _shader;


int shader_init(_shader* shader, const char* vert_shader_path, const char* frag_shader_path);
void shader_use(_shader* shader);
void shader_set_vec2(_shader* shader, const char* name, float x, float y);
void shader_set_mat4(_shader* shader, const char* name, const float* matrix);
void shader_set_int(_shader* shader, const char* name, int value);
void shader_destroy(_shader* shader);