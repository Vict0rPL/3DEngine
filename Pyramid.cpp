/**
 * @file Pyramid.cpp
 * @brief Implementacja rysowania obiektu typu Pyramid (piramida 3D).
 */
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include "Pyramid.h"
#include "Engine.h"


/**
 * @brief Rysuje piramidę 3D z teksturą lub bez.
 *
 * Funkcja renderuje piramidę składającą się z czterech trójkątnych ścian oraz kwadratowej podstawy.
 * Jeśli piramida jest teksturowana i dostępne są tekstury w silniku, odpowiednia tekstura zostanie użyta.
 * Rysowane są zarówno wypełnione ściany (z teksturą lub jednolitym kolorem), jak i krawędzie w trybie
 * drucianym (wireframe). Jeśli piramida jest zaznaczona (selected), nad rysunkiem nakładane są
 * pomarańczowe, pogrubione krawędzie.
 *
 * Szczegóły implementacyjne:
 * - Ustawia macierz modelu.
 * - Obsługuje wiązanie tekstur.
 * - Rysuje ściany i podstawę z odpowiednimi normalami i współrzędnymi tekstury.
 * - Rysuje krawędzie czarne, a gdy obiekt jest zaznaczony, dodatkowo pomarańczowe, pogrubione.
 */
void Pyramid::Draw() {
    //Bind the correct texture or unbind if none
    if (Engine::instance && !Engine::instance->textures.empty()) {
        if (IsTextured()) {
            int idx = GetTexIndex() % (int)Engine::instance->textures.size();
            Texture2D* tex = Engine::instance->textures[idx];
            if (tex && tex->GetID() != 0) {
                tex->Bind();
            }
            else {
                Texture2D::Unbind();
            }
        }
        else {
            Texture2D::Unbind();
        }
    }
    else {
        // If no textures have been loaded ensure none is bound
        Texture2D::Unbind();
    }

    // Apply this object's transform
    glm::mat4 model = GetModelMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(model));

    // Draw the filled faces (textured if bound, else flat white)
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Triangular sides
    glBegin(GL_TRIANGLES);
    // Front
    glNormal3f(0.0f, 0.4472f, 0.8944f);
    if (IsTextured()) glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    if (IsTextured()) glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    if (IsTextured()) glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    // Right
    glNormal3f(0.8944f, 0.4472f, 0.0f);
    if (IsTextured()) glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    if (IsTextured()) glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    if (IsTextured()) glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    // Back
    glNormal3f(0.0f, 0.4472f, -0.8944f);
    if (IsTextured()) glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    if (IsTextured()) glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    if (IsTextured()) glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    // Left
    glNormal3f(-0.8944f, 0.4472f, 0.0f);
    if (IsTextured()) glTexCoord2f(0.5f, 1.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    if (IsTextured()) glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    if (IsTextured()) glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();

    // square base (textured if bound, else flat white)
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    if (IsTextured()) glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    if (IsTextured()) glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    if (IsTextured()) glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    if (IsTextured()) glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glEnd();

    // Wireframe pass thin black edges for all faces
    Texture2D::Unbind();               // ensure no texture when drawing lines
    glLineWidth(1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);       // black
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Triangular sides
    glBegin(GL_TRIANGLES);
    // Front
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    // Right
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    // Back
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    // Left
    glVertex3f(0.0f, 0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();

    // Base quad edges
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glEnd();

    // Overlay thick orange edges if selected
    if (IsSelected()) {
        glLineWidth(3.0f);
        glColor3f(1.0f, 0.5f, 0.0f);   // orange

        // Triangular sides again
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.5f, 0.0f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);

        glVertex3f(0.0f, 0.5f, 0.0f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);

        glVertex3f(0.0f, 0.5f, 0.0f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, -0.5f);

        glVertex3f(0.0f, 0.5f, 0.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glEnd();

        // Base quad again
        glBegin(GL_QUADS);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glEnd();
    }

    // Restore defaults and pop matrix
    glLineWidth(1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPopMatrix();
}
