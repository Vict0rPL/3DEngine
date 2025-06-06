#include "Cube.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include "Engine.h"      // to access the texture pointer

void Cube::Draw() {
    //Bind the cube texture (if it exists)
    if (Engine::instance && Engine::instance->cubeTexture) {
        Engine::instance->cubeTexture->Bind();
    }

    //Load model matrix
    glm::mat4 model = GetModelMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(model));

    //Define cube geometry and normals
    struct V { float x, y, z; };
    static V verts[8] = {
        {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
        {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}
    };
    static int faces[6][4] = {
        {0,1,2,3}, {4,5,6,7},
        {0,1,5,4}, {2,3,7,6},
        {0,3,7,4}, {1,2,6,5}
    };
    static V norms[6] = {
        { 0,  0, -1}, { 0,  0,  1},
        { 0, -1,  0}, { 0,  1,  0},
        {-1,  0,  0}, { 1,  0,  0}
    };

    //Define per‐face texture coordinates (full quad each face)
    //    (u,v) pairs for the 4 vertices in order
    static float texCoords[4][2] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    };

    //Draw filled quads with texture
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);         // fully modulate by texture color
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    for (int i = 0; i < 6; ++i) {
        glNormal3f(norms[i].x, norms[i].y, norms[i].z);
        for (int j = 0; j < 4; ++j) {
            // Send the (u,v) before each vertex
            glTexCoord2f(texCoords[j][0], texCoords[j][1]);
            auto& v = verts[faces[i][j]];
            glVertex3f(v.x, v.y, v.z);
        }
    }
    glEnd();

    // Draw wireframe
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

    //Restore defaults
    glLineWidth(1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPopMatrix();
}
