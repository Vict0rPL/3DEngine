#pragma once
#include <GL/freeglut.h>
#include <string>

class Texture2D {
public:
    // Load a 2D texture from disk (using stb_image). On failure, `id` will be 0.
    Texture2D(const char* filepath);

    // Bind this texture (GL_TEXTURE_2D) on the active texture unit.
    void Bind() const;

    // Unbind (bind 0).
    static void Unbind();

    // Delete the GL texture.
    void   Delete();

    // Getters
    GLuint GetID() const { return id; }
    int    GetWidth()  const { return width; }
    int    GetHeight() const { return height; }

private:
    GLuint id = 0;
    int    width = 0;
    int    height = 0;
    int    numChan = 0;
};
