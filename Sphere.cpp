#include "Sphere.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

void Sphere::Draw() {
    // Załaduj macierz modelu
    glm::mat4 model = GetModelMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(model));

    // Kolor sfery
    glColor3f(0.8f, 0.2f, 0.2f);

    // Wypełniona sfera o promieniu 0.5, 16 segm. wzdłuż i wszerz
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glutSolidSphere(0.5, 16, 16);

    // Czarna obwódka (wireframe)
    glColor3f(0.0f, 0.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glutWireSphere(0.5, 16, 16);

    // Przywróć wypełnianie i macierz
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPopMatrix();
}
