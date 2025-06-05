#include "Engine.h"
#include "Cube.h"
#include "Pyramid.h"
#include "Sphere.h"

#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Initialize the static instance pointer to nullptr
Engine* Engine::instance = nullptr;

Engine::Engine(int argc, char** argv)
    : width(800),
    height(600),
    fullscreen(false),
    window(0),
    fps(60),
    clearColor(0.0f, 0.0f, 0.0f),
    projMode(ProjectionMode::Perspective),
    fov(45.0f),
    zNear(0.1f),
    zFar(100.0f),
    orthoLeft(-1.0f),
    orthoRight(1.0f),
    orthoBottom(-1.0f),
    orthoTop(1.0f),
    angleY(0.0f),
    angleX(0.0f),
    camDist(5.0f),
    camTarget(0.0f, 0.0f, 0.0f),
    lastMouseX(-1),
    lastMouseY(-1),
    rotating(false),
    lightingEnabled(true),
    shadingEnabled(true),
    selectedIndex(-1)
{
    // Initialize GLUT with command‐line arguments:
    glutInit(&argc, argv);

    // Set the singleton instance pointer
    instance = this;
}

Engine::~Engine() {
    // Delete all allocated scene objects
    for (auto obj : objects) {
        delete obj;
    }
    objects.clear();
}

void Engine::Init() {
    // Set up display mode: double buffering, RGB, depth buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    window = glutCreateWindow("3D Engine");

    if (fullscreen) {
        glutFullScreen();
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set initial shade model
    glShadeModel(GL_SMOOTH);

    // Lighting setup
    glEnable(GL_LIGHTING);     // global lighting enable
    glEnable(GL_LIGHT0);       // use light

    // Allow glColor to set ambient & diffuse material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Normalize normals after modelview transforms
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    // Configure light
    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat position[] = { 10.0f, 10.0f, 10.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    // Create the initial scene object and select it
    objects.push_back(new Cube());
    selectedIndex = 0;  // first object is selected

    // Register GLUT callbacks
    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutSpecialFunc(SpecialCallback);
    glutMouseFunc(MouseCallback);
    glutMotionFunc(MotionCallback);
    glutTimerFunc(0, TimerCallback, 0);
}

void Engine::Run() {
    // Enter the GLUT event loop
    glutMainLoop();
}

void Engine::Cleanup() {
    std::cout << "Cleaning up...\n";
    if (window != 0) {
        glutDestroyWindow(window);
        window = 0;
    }
}

//   Configuration setters
void Engine::SetResolution(int w, int h) {
    width = w;
    height = h;
}

void Engine::SetFullscreen(bool value) {
    fullscreen = value;
}

void Engine::SetClearColor(float r, float g, float b) {
    clearColor = glm::vec3(r, g, b);
    glClearColor(r, g, b, 1.0f);
}

void Engine::SetFPS(int frames) {
    fps = frames;
}


//   Projection setters
void Engine::SetPerspective(float fovDeg, float zn, float zf) {
    projMode = ProjectionMode::Perspective;
    fov = fovDeg;
    zNear = zn;
    zFar = zf;
}

void Engine::SetOrtho(float left, float right, float bottom, float top, float zn, float zf) {
    projMode = ProjectionMode::Ortho;
    orthoLeft = left;
    orthoRight = right;
    orthoBottom = bottom;
    orthoTop = top;
    zNear = zn;
    zFar = zf;
}

//   Display callback
void Engine::Display() {
    // Clear color & depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Build camera (view) matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glm::mat4 view = glm::lookAt(
        glm::vec3(
            camDist * sin(angleY) * cos(angleX) + camTarget.x,
            camDist * sin(angleX) + camTarget.y,
            camDist * cos(angleY) * cos(angleX) + camTarget.z
        ),
        camTarget,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    glLoadMatrixf(glm::value_ptr(view));

    // Mark exactly one object as “selected”
    for (int i = 0; i < (int)objects.size(); ++i) {
        objects[i]->SetSelected(i == selectedIndex);
    }

    // Draw all objects
    for (auto obj : objects) {
        obj->Draw();
    }

    // Swap buffers
    glutSwapBuffers();
}


//   Reshape callback
void Engine::Reshape(int w, int h) {
    width = w;
    height = h;
    if (h == 0) {
        h = 1;
    }
    float aspect = (float)w / (float)h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (projMode == ProjectionMode::Perspective) {
        gluPerspective(fov, aspect, zNear, zFar);
    }
    else {
        glOrtho(orthoLeft * aspect,
            orthoRight * aspect,
            orthoBottom,
            orthoTop,
            zNear,
            zFar);
    }
}


//   Keyboard callback
void Engine::Keyboard(unsigned char key, int, int) {
    const float moveStep = 0.1f;

    switch (key) {
    case 27: // ESC
        exit(0);
        break;

    case '\t': // Tab: cycle selection
        if (!objects.empty()) {
            selectedIndex = (selectedIndex + 1) % (int)objects.size();
            std::cout << "Selected object index = " << selectedIndex << "\n";
        }
        break;

    case 'p': // Switch to perspective
        SetPerspective(fov, zNear, zFar);
        Reshape(width, height);
        break;

    case 'o': // Switch to orthographic
        SetOrtho(orthoLeft, orthoRight, orthoBottom, orthoTop, zNear, zFar);
        Reshape(width, height);
        break;

    case 'l': // Toggle lighting
        lightingEnabled = !lightingEnabled;
        if (lightingEnabled) glEnable(GL_LIGHTING);
        else                glDisable(GL_LIGHTING);
        break;

    case 'h': // Toggle shading model
        shadingEnabled = !shadingEnabled;
        if (shadingEnabled)
            glShadeModel(GL_SMOOTH);
        else
            glShadeModel(GL_FLAT);
        break;

    case 'w': // Pan camera up
        camTarget.y += moveStep;
        break;
    case 's': // Pan camera down
        camTarget.y -= moveStep;
        break;
    case 'a': // Pan camera left
        camTarget.x -= moveStep;
        break;
    case 'd': // Pan camera right
        camTarget.x += moveStep;
        break;
    case 'q': // Move camera forward
        camTarget.z += moveStep;
        break;
    case 'e': // Move camera backward
        camTarget.z -= moveStep;
        break;

    case '1': { // Add a new Cube at camTarget
        Cube* newCube = new Cube();
        newCube->SetPosition(camTarget);
        objects.push_back(newCube);
        selectedIndex = (int)objects.size() - 1;
        break;
    }
    case '2': { // Add a new Pyramid at camTarget
        Pyramid* newPyr = new Pyramid();
        newPyr->SetPosition(camTarget);
        objects.push_back(newPyr);
        selectedIndex = (int)objects.size() - 1;
        break;
    }
    case '3': { // Add a new Sphere at camTarget
        Sphere* newSphere = new Sphere();
        newSphere->SetPosition(camTarget);
        objects.push_back(newSphere);
        selectedIndex = (int)objects.size() - 1;
        break;
    }

    default:
        break;
    }

    // Request redisplay
    glutPostRedisplay();
}


//   Special keys callback (e.g. arrow keys). Not used right now.
void Engine::Special(int key, int, int) {
    // Placeholder for future object‐manipulation via arrow keys, etc.
    glutPostRedisplay();
}


//   Mouse button callback (for rotating & zooming)
void Engine::Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        rotating = (state == GLUT_DOWN);
        lastMouseX = x;
        lastMouseY = y;
    }
    else if (button == 3) { // scroll up
        camDist = std::max(0.5f, camDist - 0.5f);
    }
    else if (button == 4) { // scroll down
        camDist += 0.5f;
    }
    glutPostRedisplay();
}


//   Mouse motion callback (dragging to rotate camera)
void Engine::Motion(int x, int y) {
    if (rotating) {
        angleY += (x - lastMouseX) * 0.005f;
        angleX -= (y - lastMouseY) * 0.005f;
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}


//instance‐side timer handler
void Engine::OnTimer(int value) {
    glutPostRedisplay();
    // Schedule the next timer callback
    glutTimerFunc(1000 / fps, TimerCallback, 0);
}


//   Static GLUT callback wrapper for timer
void Engine::TimerCallback(int value) {
    // Call the renamed instance method
    instance->OnTimer(value);
}


//   Static GLUT callback wrappers for the rest
void Engine::DisplayCallback() {
    instance->Display();
}

void Engine::ReshapeCallback(int w, int h) {
    instance->Reshape(w, h);
}

void Engine::KeyboardCallback(unsigned char k, int x, int y) {
    instance->Keyboard(k, x, y);
}

void Engine::SpecialCallback(int key, int x, int y) {
    instance->Special(key, x, y);
}

void Engine::MouseCallback(int button, int state, int x, int y) {
    instance->Mouse(button, state, x, y);
}

void Engine::MotionCallback(int x, int y) {
    instance->Motion(x, y);
}
