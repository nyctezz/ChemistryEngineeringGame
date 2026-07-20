#include "mesh.h"

void mesh_init(_mesh* mesh, float* vertices, size_t vertex_count, uint32_t* indices, size_t index_count)
{
    mesh->vertices = vertices;
    mesh->indices = indices;
    mesh->vertex_count = vertex_count;
    mesh->index_count = index_count;

    glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ebo);

    glBindVertexArray(mesh->vao);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(float), vertices, GL_STATIC_DRAW);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(uint32_t), indices, GL_STATIC_DRAW);

    // Position attributes
    // Attribute 0: Position (3 floats, stride = 5 * sizeof(float))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Attribute 1: Texture Coordinates (2 floats, stride = 5 * sizeof(float), offset = 3 floats)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void mesh_init_worldtile(_mesh* mesh)
{
    /*
    // Define vertices for a quad (two triangles) with positions and texture coordinates
    float vertices[] = {
        // Positions          // Texture Coords (UV)
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom-left
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom-right
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top-right
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // Top-left
    };

    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    */

    //float vertices[] = {
    //    // Position                  // UV
    //    0.0f,  0.0f, 0.0f,          0.5f,  1.0f,
//
    //    0.5f, -0.8660254f, 0.0f,    1.0f, -0.0f,
    //    -0.5f, -0.8660254f, 0.0f,   -0.0f, -0.0f,
    //    -1.0f,  0.0f, 0.0f,         x,  x,
    //    -0.5f,  0.8660254f, 0.0f,    x,  x,
    //    0.5f,  0.8660254f, 0.0f,    x,  x,
    //    1.0f,  0.0f, 0.0f,          x,  x
    //};

    float vertices[] = {
        // Position                  // UV
        0.0f,  0.0f, 0.0f,          0.5f, 1.0f,

        0.5f, -0.8660254f, 0.0f,    1.0f, 0.0f,
        -0.5f, -0.8660254f, 0.0f,    0.0f, 0.0f,

        -1.0f,  0.0f, 0.0f,         -0.5f, 1.0f,

        // Flipped vertically
        -0.5f,  0.8660254f, 0.0f,    0.0f, 0.0f,
        0.5f,  0.8660254f, 0.0f,    1.0f, 0.0f,
        1.0f,  0.0f, 0.0f,          1.5f, 1.0f
    };

    uint32_t indices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 1
    };

    mesh_init(mesh, vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(uint32_t));
}

void mesh_draw(const _mesh* mesh)
{
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

void mesh_destroy(_mesh* mesh)
{
    glDeleteBuffers(1, &mesh->ebo);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteVertexArrays(1, &mesh->vao);
}