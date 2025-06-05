#include "Cube.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

void Cube::Draw() {
    //Load model matrix
    glm::mat4 model = GetModelMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(model));

    //Draw filled faces
    struct V { float x, y, z; };
    static V verts[8] = {
        {-0.5f,-0.5f,-0.5f},{ 0.5f,-0.5f,-0.5f},{ 0.5f, 0.5f,-0.5f},{-0.5f, 0.5f,-0.5f},
        {-0.5f,-0.5f, 0.5f},{ 0.5f,-0.5f, 0.5f},{ 0.5f, 0.5f, 0.5f},{-0.5f, 0.5f, 0.5f}
    };
    static int faces[6][4] = {
        {0,1,2,3}, {4,5,6,7},
        {0,1,5,4}, {2,3,7,6},
        {0,3,7,4}, {1,2,6,5}
    };
    static V norms[6] = {
        { 0, 0,-1}, { 0, 0, 1},
        { 0,-1, 0}, { 0, 1, 0},
        {-1, 0, 0}, { 1, 0, 0}
    };

    glColor3f(0.2f, 0.6f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    for (int i = 0; i < 6; ++i) {
        glNormal3f(norms[i].x, norms[i].y, norms[i].z);
        for (int j = 0; j < 4; ++j) {
            auto& v = verts[faces[i][j]];
            glVertex3f(v.x, v.y, v.z);
        }
    }
    glEnd();

    //Wireframe pass: thick orange if selected, else thin black
    if (IsSelected()) {
        glLineWidth(3.0f);                // thicker lines
        glColor3f(1.0f, 0.5f, 0.0f);     // bright orange
    }
    else {
        glLineWidth(1.0f);
        glColor3f(0.0f, 0.0f, 0.0f);     // black
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

    //Restore defaults
    glLineWidth(1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPopMatrix();
}
