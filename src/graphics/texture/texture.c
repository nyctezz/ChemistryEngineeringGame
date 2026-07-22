#include "texture.h"

uint32_t texture_load_png(const char* filepath)
{
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 4);

    // fall back to the debug texture if loading failed:
    if (!data)
    {
        fprintf(stderr, "Failed to load texture '%s': %s\n", filepath, stbi_failure_reason());

        const char* debug_filepath = "assets/textures/debug.png";

        data = stbi_load(debug_filepath, &width, &height, &nrChannels, 4);

        // return 0 if the debug texture also fails to load:
        if (!data)
        {
            fprintf(stderr, "Failed to load debug texture '%s': %s\n", debug_filepath, stbi_failure_reason());
            return 0;
        }

        filepath = debug_filepath;
    }

    uint32_t texture_id;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Texture wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Pixel-art filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Upload the pixel data to the GPU.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Clean up CPU memory.
    stbi_image_free(data);

    return texture_id;
}