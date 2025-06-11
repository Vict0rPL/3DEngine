/**
 * @file Texture2D.h
 * @brief Klasa pomocnicza do ładowania i obsługi tekstur 2D w OpenGL.
 */

#pragma once

#include <GL/freeglut.h>
#include <string>

 /**
  * @class Texture2D
  * @brief Reprezentuje teksturę 2D w OpenGL.
  *
  * Ładuje teksturę z pliku przy użyciu biblioteki `stb_image`, udostępnia metody jej bindowania oraz usuwania.
  */
class Texture2D {
public:
    /**
     * @brief Konstruktor – ładuje teksturę z podanej ścieżki.
     *
     * Jeśli wczytanie się nie powiedzie, `id == 0`.
     * @param filepath Ścieżka do pliku z teksturą (np. PNG, JPG)
     */
    Texture2D(const char* filepath);

    /**
     * @brief Binguje tę teksturę jako `GL_TEXTURE_2D` na aktywnej jednostce teksturującej.
     */
    void Bind() const;

    /**
     * @brief Odwiązuje teksturę (binduje `0`).
     */
    static void Unbind();

    /**
     * @brief Usuwa teksturę z pamięci GPU (jeśli została załadowana).
     */
    void Delete();

    // ===== Gettery =====

    /**
     * @brief Zwraca identyfikator OpenGL tekstury.
     * @return ID tekstury (GLuint)
     */
    GLuint GetID() const { return id; }

    /**
     * @brief Zwraca szerokość tekstury w pikselach.
     */
    int GetWidth() const { return width; }

    /**
     * @brief Zwraca wysokość tekstury w pikselach.
     */
    int GetHeight() const { return height; }

private:
    GLuint id = 0;      ///< ID tekstury w OpenGL
    int width = 0;      ///< Szerokość tekstury
    int height = 0;     ///< Wysokość tekstury
    int numChan = 0;    ///< Liczba kanałów (np. 3 = RGB, 4 = RGBA)
};
