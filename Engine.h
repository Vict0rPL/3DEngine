// Engine.h
#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Object3D.h"

class Engine {
public:
    enum class ProjectionMode { Perspective, Ortho };

    Engine(int argc, char** argv);
    ~Engine();
    void Init();
    void Run();
    void Cleanup();

    // Window and graphics mode
    void SetResolution(int w, int h);
    void SetFullscreen(bool value);

    // Clear color
    void SetClearColor(float r, float g, float b);

    // Animation speed
    void SetFPS(int frames);

    // Projection settings
    void SetPerspective(float fovDeg, float zn, float zf);
    void SetOrtho(float left, float right, float bottom, float top, float zn, float zf);

    static Engine* instance;

private:
    int width, height, fps;
    int window;
    bool fullscreen;
    glm::vec3 clearColor;
    
    // Light
    bool lightingEnabled;
    // shading
    bool shadingEnabled;
    // Projection
    ProjectionMode projMode;
    float fov, zNear, zFar;
    float orthoLeft, orthoRight, orthoBottom, orthoTop;

    // Camera control
    float angleY, angleX, camDist;
    glm::vec3 camTarget;
    int lastMouseX, lastMouseY;
    bool rotating;

    // Scene objects
    std::vector<Object3D*> objects;

    // Core loop and callbacks
    void Display();
    void Reshape(int w, int h);
    void Keyboard(unsigned char key, int x, int y);
    void Special(int key, int x, int y);
    void Mouse(int button, int state, int x, int y);
    void Motion(int x, int y);

    static void DisplayCallback();
    static void ReshapeCallback(int w, int h);
    static void KeyboardCallback(unsigned char key, int x, int y);
    static void SpecialCallback(int key, int x, int y);
    static void MouseCallback(int button, int state, int x, int y);
    static void MotionCallback(int x, int y);
    static void TimerCallback(int value);

};