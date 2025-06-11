/**
 * @file Sphere.cpp
 * @brief Implementacja rysowania obiektu typu Sphere (sfera 3D).
 */
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include "Sphere.h"
#include "Engine.h"

/**
 * @brief Rysuje sferę 3D z teksturą lub bez.
 *
 * Funkcja renderuje sferę o promieniu 0.5, złożoną z 32 plaszczyzn wzdłuż "slices" i "stacks".
 * Jeśli obiekt jest teksturowany i dostępne są tekstury w silniku, odpowiednia tekstura zostanie użyta.
 * Rysowana jest zarówno wypełniona sfera (teksturą lub jednolitym kolorem), jak i nakładka drucianej siatki.
 *
 * Szczegóły implementacyjne:
 * - Ustawia macierz modelu (transformację).
 * - Wiąże odpowiednią teksturę lub odwiązuje tekstury, jeśli nie ma ich dostępnych.
 * - Używa GLU quadric do wygenerowania sfery z automatycznie generowanymi współrzędnymi tekstury.
 * - Rysuje overlay w trybie drucianym (wireframe), pomarańczowy jeśli zaznaczona, czarny w przeciwnym wypadku.
 */
void Sphere::Draw() {
    // Bind the correct texture (if any) for this object
    if (Engine::instance && !Engine::instance->textures.empty()) {
        if (IsTextured()) {
            // Get this object's texture index (wrapped)
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
        // No textures loaded at all, so just ensure none are bound
        Texture2D::Unbind();
    }

    // Push the model matrix
    glm::mat4 model = GetModelMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(model));


    glColor3f(1.0f, 1.0f, 1.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Create a GLU quadric so we can auto‐generate texture coordinates
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    // Tell GLU to generate (s,t) texture coords for the sphere
    gluQuadricTexture(quad, GL_TRUE);

    // Draw the textured (or flat) sphere
    //   radius = 0.5, slices = 32, stacks = 32
    gluSphere(quad, 0.5, 32, 32);

    // Free the quadric
    gluDeleteQuadric(quad);

    // Draw the wireframe overlay (always untextured)
    Texture2D::Unbind();
    if (IsSelected()) {
        glLineWidth(3.0f);
        glColor3f(1.0f, 0.5f, 0.0f);  // bright orange outline if selected
    }
    else {
        glLineWidth(1.0f);
        glColor3f(0.0f, 0.0f, 0.0f);  // black outline otherwise
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // We can draw a wireframe sphere with a second quadric or with glutWireSphere
    glutWireSphere(0.5, 16, 16);

    // Restore defaults
    glLineWidth(1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glPopMatrix();
}
