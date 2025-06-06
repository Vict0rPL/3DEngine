// Texture2D.cpp
#include <GL/glew.h>         // must come before freeglut.h
#include <GL/freeglut.h>
#include "Texture2D.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture2D::Texture2D(const char* filepath)
{
    // 1) Load the image from disk with stb_image
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filepath, &width, &height, &numChan, 0);
    if (!data) {
        std::cerr << "Failed to load texture \"" << filepath << "\"\n";
        return; // `id` remains 0
    }

    // 2) Figure out format (GL_RED, GL_RGB, or GL_RGBA)
    GLenum format = GL_RGB;
    if (numChan == 1) format = GL_RED;
    else if (numChan == 3) format = GL_RGB;
    else if (numChan == 4) format = GL_RGBA;
    else {
        std::cerr << "Unsupported channel count (" << numChan
            << ") in texture \"" << filepath << "\"\n";
        stbi_image_free(data);
        return;
    }

    // 3) Generate a single GL texture name and bind it:
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // 4) Upload the pixel data to the GPU (level 0):
    glTexImage2D(
        GL_TEXTURE_2D,
        0,             // mipmap level
        format,        // internal format
        width, height,
        0,             // border (must be 0)
        format,        // data format
        GL_UNSIGNED_BYTE,
        data           // pointer to the image data
    );

    // 5) Generate mipmaps (requires a loaded loader like GLEW):
    glGenerateMipmap(GL_TEXTURE_2D);

    // 6) Set wrap/filter parameters (trilinear min, bilinear mag):
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 7) Unbind and free the CPU memory:
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Delete()
{
    if (id != 0) {
        glDeleteTextures(1, &id);
        id = 0;
    }
}
