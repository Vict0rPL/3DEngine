#include "Sphere.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

void Sphere::Draw() {
    //Load model matrix
    glm::mat4 model = GetModelMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(model));

    //Filled pass
    glColor3f(0.8f, 0.2f, 0.2f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glutSolidSphere(0.5, 16, 16);

    //Wireframe pass
    if (IsSelected()) {
        glLineWidth(3.0f);
        glColor3f(1.0f, 0.5f, 0.0f);  // bright orange
    }
    else {
        glLineWidth(1.0f);
        glColor3f(0.0f, 0.0f, 0.0f);  // black
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glutWireSphere(0.5, 16, 16);

    //Restore defaults
    glLineWidth(1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPopMatrix();
}
