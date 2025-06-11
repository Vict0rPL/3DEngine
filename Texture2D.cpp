/**
 * @file Texture2D.cpp
 * @brief Implementacja klasy Texture2D obsługującej tekstury 2D w OpenGL.
 */
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Texture2D.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

 /**
  * @brief Konstruktor klasy Texture2D.
  *
  * Ładuje teksturę z pliku o podanej ścieżce, generuje identyfikator OpenGL,
  * przesyła dane do GPU i ustawia parametry tekstury (wrap, filtrowanie, mipmapy).
  *
  * @param filepath Ścieżka do pliku obrazu tekstury.
  */

Texture2D::Texture2D(const char* filepath)
{
    // Load the image from disk with stb_image
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filepath, &width, &height, &numChan, 0);
    if (!data) {
        std::cerr << "Failed to load texture \"" << filepath << "\"\n";
        return; // id remains 0
    }

    //Figure out format (GL_RED, GL_RGB, or GL_RGBA)
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

    // Generate a single GL texture name and bind it
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Upload the pixel data to the GPU (level 0)
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

    // Generate mipmaps 
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set wrap/filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Unbind and free the CPU memory
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

/**
 * @brief Wiąże (binduje) teksturę do aktualnego kontekstu OpenGL.
 *
 * Po wywołaniu tej funkcji tekstura staje się aktywna i może być używana do renderowania.
 */
void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}

/**
 * @brief Odwiązuje (unbinduje) dowiązaną teksturę 2D.
 *
 * Sprawia, że żadna tekstura 2D nie jest aktualnie aktywna.
 */
void Texture2D::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * @brief Usuwa teksturę z GPU i zeruje jej identyfikator.
 *
 * Powinno się wywołać przed usunięciem obiektu lub gdy tekstura nie jest już potrzebna.
 */
void Texture2D::Delete()
{
    if (id != 0) {
        glDeleteTextures(1, &id);
        id = 0;
    }
}
