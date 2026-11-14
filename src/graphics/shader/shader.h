#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <cglm/cglm.h>

typedef struct {
    unsigned int vertex_shader;
    unsigned int fragment_shader;
    unsigned int shader_program;

    // fix for shader_set_vec2 < string lookup, EVERY call. To fix it we cache everything it uses:
    GLint uTranslation_loc;
    GLint uProjection_loc;
    GLint uView_loc;
    GLint uColorTint_loc;
} _shader;


int shader_init(_shader* shader, const char* vert_shader_path, const char* frag_shader_path);
void shader_use(_shader* shader);
void shader_set_vec2(_shader* shader, const char* name, float x, float y);
void shader_set_vec4(const _shader* shader, const char* name, float x, float y, float z, float w);
void shader_set_mat4(_shader* shader, const char* name, mat4 matrix);
void shader_set_int(_shader* shader, const char* name, int value);
void shader_destroy(_shader* shader);