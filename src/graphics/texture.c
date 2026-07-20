#include "texture.h"

uint32_t texture_load_png(const char* filepath) {
    int width, height, nrChannels;
    
    // 1. Flip the texture vertically so it's not upside down
    stbi_set_flip_vertically_on_load(true);

    // 2. Load the pixel data (force RGBA with 4 channels)
    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 4);
    if (!data) {
        printf("Failed to load texture at path: %s\n", filepath);
        return 0;
    }

    // 3. Generate and bind the OpenGL texture
    uint32_t texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // 4. Set texture wrapping and filtering options
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //smoothing:
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //raw pixels:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // 5. Upload the pixel data to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // 6. Clean up CPU memory
    stbi_image_free(data);

    return texture_id;
}