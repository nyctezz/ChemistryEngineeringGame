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