/**
 * @file Cube.cpp
 * @brief Implementacja metody Draw klasy Cube – rysowanie sześcianu 3D z obsługą tekstur i zaznaczenia.
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include "Cube.h"
#include "Engine.h"

 /**
  * @brief Rysuje sześcian 3D z uwzględnieniem transformacji, teksturowania i zaznaczenia.
  *
  * Funkcja:
  * - Wczytuje macierz modelu obiektu.
  * - Rysuje sześcian z włączonym teksturowaniem (jeśli aktywne).
  * - Dodaje siatkę (wireframe), jeżeli obiekt jest zaznaczony.
  * - Obsługuje rysowanie ścian z normalami i współrzędnymi UV.
  */
void Cube::Draw() {
    // Jeśli silnik działa i ma załadowane tekstury
    if (Engine::instance && Engine::instance->textures.size() > 0) {
        if (IsTextured()) {
            int idx = GetTexIndex();
            idx = idx % (int)Engine::instance->textures.size();
            Texture2D* tex = Engine::instance->textures[idx];
            if (tex && tex->GetID() != 0) {
                tex->Bind();
            }
        }
        else {
            Texture2D::Unbind();
        }
    }

    // Pobierz i ustaw macierz modelu
    glm::mat4 model = GetModelMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(model));

    // Definicje wierzchołków, ścian i normalnych
    struct V { float x, y, z; };
    static V verts[8] = {
        {-0.5f,-0.5f,-0.5f},{ 0.5f,-0.5f,-0.5f},
        { 0.5f, 0.5f,-0.5f},{-0.5f, 0.5f,-0.5f},
        {-0.5f,-0.5f, 0.5f},{ 0.5f,-0.5f, 0.5f},
        { 0.5f, 0.5f, 0.5f},{-0.5f, 0.5f, 0.5f}
    };
    static int faces[6][4] = {
        {0,1,2,3},{4,5,6,7},
        {0,1,5,4},{2,3,7,6},
        {0,3,7,4},{1,2,6,5}
    };
    static V norms[6] = {
        { 0,  0, -1},{ 0,  0,  1},
        { 0, -1,  0},{ 0,  1,  0},
        {-1,  0,  0},{ 1,  0,  0}
    };
    static float texCoords[4][2] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    };

    // Rysowanie wypełnionego, teksturowanego sześcianu
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    for (int i = 0; i < 6; ++i) {
        glNormal3f(norms[i].x, norms[i].y, norms[i].z);
        for (int j = 0; j < 4; ++j) {
            glTexCoord2f(texCoords[j][0], texCoords[j][1]);
            auto& v = verts[faces[i][j]];
            glVertex3f(v.x, v.y, v.z);
        }
    }
    glEnd();

    // Rysowanie siatki (wireframe), jeśli obiekt jest zaznaczony
    Texture2D::Unbind();
    if (IsSelected()) {
        glLineWidth(3.0f);
        glColor3f(1.0f, 0.5f, 0.0f);
    }
    else {
        glLineWidth(1.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_QUADS);
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            auto& v = verts[faces[i][j]];
            glVertex3f(v.x, v.y, v.z);
        }
    }
    glEnd();

    // Przywróć domyślne ustawienia
    glLineWidth(1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPopMatrix();
}
