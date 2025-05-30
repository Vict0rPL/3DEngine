// Engine.cpp
#include "Engine.h"
#include "Cube.h"
#include "Pyramid.h"
#include "Sphere.h"
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Engine* Engine::instance = nullptr;

Engine::Engine(int argc, char** argv)
    : width(800), height(600), fullscreen(false), fps(60), clearColor(0.0f),
    projMode(ProjectionMode::Perspective), fov(45.0f), zNear(0.1f), zFar(100.0f),
    orthoLeft(-1.0f), orthoRight(1.0f), orthoBottom(-1.0f), orthoTop(1.0f),
    angleY(0.0f), angleX(0.0f), camDist(5.0f), camTarget(0.0f), lastMouseX(-1), lastMouseY(-1), rotating(false), shadingEnabled(true)
{
    glutInit(&argc, argv);
}

Engine::~Engine() {
    for (auto obj : objects) delete obj;
}

void Engine::Init() {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    window = glutCreateWindow("3D Engine");
    if (fullscreen) glutFullScreen();

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    // --- new lighting setup ---
    lightingEnabled = true;
    glEnable(GL_LIGHTING);    // enable overall lighting :contentReference[oaicite:0]{index=0}
    glEnable(GL_LIGHT0);      // enable light #0 :contentReference[oaicite:1]{index=1}

    // tell GL to let glColor() drive the material’s ambient+diffuse
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // normalize normals & smooth shading
    glEnable(GL_NORMALIZE);     // normalize normals after modelview transforms :contentReference[oaicite:2]{index=2}
    glShadeModel(GL_SMOOTH);

    // set up light-0’s ambient, diffuse, specular and position
    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat position[] = { 10.0f, 10.0f, 10.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    // ----------------------------------

    // Create scene: a cube
    objects.push_back(new Cube());

    // Register callbacks
    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutSpecialFunc(SpecialCallback);
    glutMouseFunc(MouseCallback);
    glutMotionFunc(MotionCallback);
    glutTimerFunc(0, TimerCallback, 0);
}

void Engine::Run() { glutMainLoop(); }

void Engine::Cleanup() {
    std::cout << "Cleaning up...\n";
    glutDestroyWindow(window);
}

void Engine::SetResolution(int w, int h) { width = w; height = h; }
void Engine::SetFullscreen(bool value) { fullscreen = value; }
void Engine::SetClearColor(float r, float g, float b) { clearColor = { r, g, b }; }
void Engine::SetFPS(int frames) { fps = frames; }

void Engine::SetPerspective(float fovDeg, float zn, float zf) {
    projMode = ProjectionMode::Perspective; fov = fovDeg; zNear = zn; zFar = zf;
}

void Engine::SetOrtho(float left, float right, float bottom, float top, float zn, float zf) {
    projMode = ProjectionMode::Ortho; orthoLeft = left; orthoRight = right; orthoBottom = bottom; orthoTop = top; zNear = zn; zFar = zf;
}

void Engine::Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glm::mat4 view = glm::lookAt(
        glm::vec3(camDist * sin(angleY) * cos(angleX) + camTarget.x,
            camDist * sin(angleX) + camTarget.y,
            camDist * cos(angleY) * cos(angleX) + camTarget.z),
        camTarget, glm::vec3(0, 1, 0));
    glLoadMatrixf(glm::value_ptr(view));
    for (auto obj : objects) obj->Draw();
    glutSwapBuffers();
}

void Engine::Reshape(int w, int h) {
    width = w; height = h; float aspect = (float)w / h; glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    if (projMode == ProjectionMode::Perspective) gluPerspective(fov, aspect, zNear, zFar);
    else glOrtho(orthoLeft * aspect, orthoRight * aspect, orthoBottom, orthoTop, zNear, zFar);
}

void Engine::Keyboard(unsigned char key, int, int) { 
    const float m = 0.1f;
    switch (key) {
    case 27:  // ESC key
        exit(0);
        break;

    case 'p': // Perspective mode
        SetPerspective(fov, zNear, zFar);
        Reshape(width, height);
        break;

    case '1': { // Add another cube at the camera’s look-at point
        Cube* newCube = new Cube();
        newCube->SetPosition(camTarget);
        objects.push_back(newCube);
        break;
    }
    case '2': {
        Pyramid* newPyr = new Pyramid();
        newPyr->SetPosition(camTarget);
        objects.push_back(newPyr);
        break;
    }
    case '3': {
        Sphere* newSphere = new Sphere();
        newSphere->SetPosition(camTarget);
        objects.push_back(newSphere);
        break;
    }

    case 'o': // Orthographic mode
        SetOrtho(orthoLeft, orthoRight, orthoBottom, orthoTop, zNear, zFar);
        Reshape(width, height);
        break;

    case 'l':  // toggle lighting
        lightingEnabled = !lightingEnabled;
        if (lightingEnabled) glEnable(GL_LIGHTING);
        else                glDisable(GL_LIGHTING);
        break;

    case 'h':  // H = toggle shading
        shadingEnabled = !shadingEnabled;
        if (shadingEnabled)
            glShadeModel(GL_SMOOTH);
        else
            glShadeModel(GL_FLAT);
        break;

    // WSAD camera pan
    case 'w': camTarget.y += m; break;
    case 's': camTarget.y -= m; break;
    case 'a': camTarget.x -= m; break;
    case 'd': camTarget.x += m; break;
    case 'q': camTarget.z += m; break;  // forward
    case 'e': camTarget.z -= m; break;  // backward
    default:
        // Unhandled key
        break;
    }


    glutPostRedisplay(); }

void Engine::Special(int key, int, int) { 
    const float m = 0.1f; 
    if (key == GLUT_KEY_UP)camTarget.y += m;
    if (key == GLUT_KEY_DOWN)camTarget.y -= m; 
    if (key == GLUT_KEY_LEFT)camTarget.x -= m; 
    if (key == GLUT_KEY_RIGHT)camTarget.x += m; 
    glutPostRedisplay(); 
}

void Engine::Mouse(int b, int s, int x, int y) { if (b == GLUT_LEFT_BUTTON) { rotating = (s == GLUT_DOWN); lastMouseX = x; lastMouseY = y; } else if (b == 3)camDist = std::max(0.5f, camDist - 0.5f); else if (b == 4)camDist += 0.5f; glutPostRedisplay(); }

void Engine::Motion(int x, int y) { if (rotating) { angleY += (x - lastMouseX) * 0.005f; angleX += (y - lastMouseY) * 0.005f; lastMouseX = x; lastMouseY = y; glutPostRedisplay(); } }

void Engine::TimerCallback(int) { glutPostRedisplay(); glutTimerFunc(1000 / instance->fps, TimerCallback, 0); }
void Engine::DisplayCallback() { instance->Display(); }void Engine::ReshapeCallback(int w, int h) { instance->Reshape(w, h); }void Engine::KeyboardCallback(unsigned char k, int x, int y) { instance->Keyboard(k, x, y); }void Engine::SpecialCallback(int key, int x, int y) { instance->Special(key, x, y); }void Engine::MouseCallback(int b, int s, int x, int y) { instance->Mouse(b, s, x, y); }void Engine::MotionCallback(int x, int y) { instance->Motion(x, y); }
