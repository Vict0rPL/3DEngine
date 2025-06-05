#pragma once

#include <vector>
#include <glm/glm.hpp>

class Object3D;

class Engine {
public:
    static Engine* instance;

    // Constructor / destructor
    Engine(int argc, char** argv);
    ~Engine();

    // Initialize GLUT, OpenGL state, and create the first object
    void Init();

    // Enter the GLUT main loop
    void Run();

    // Clean up (delete window, etc.)
    void Cleanup();

    // Configuration setters
    void SetResolution(int w, int h);
    void SetFullscreen(bool value);
    void SetClearColor(float r, float g, float b);
    void SetFPS(int frames);

    // Projection mode setters
    void SetPerspective(float fovDeg, float zn, float zf);
    void SetOrtho(float left, float right, float bottom, float top, float zn, float zf);

    // GLUT callback hooks (static)
    static void DisplayCallback();
    static void ReshapeCallback(int w, int h);
    static void KeyboardCallback(unsigned char k, int x, int y);
    static void SpecialCallback(int key, int x, int y);
    static void MouseCallback(int button, int state, int x, int y);
    static void MotionCallback(int x, int y);
    static void TimerCallback(int value);

private:
    // Disallow copying
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    // Internal handlers for each callback
    void Display();
    void Reshape(int w, int h);
    void Keyboard(unsigned char key, int x, int y);
    void Special(int key, int x, int y);
    void Mouse(int button, int state, int x, int y);
    void Motion(int x, int y);

    // timer handler
    void OnTimer(int value);

    // Window / context state
    int width, height;
    bool fullscreen;
    int window;       // GLUT window handle

    // Timing
    int fps;

    // Clear color
    glm::vec3 clearColor;

    // Projection parameters
    enum class ProjectionMode { Perspective, Ortho };
    ProjectionMode projMode;
    float fov, zNear, zFar;
    float orthoLeft, orthoRight, orthoBottom, orthoTop;

    // Camera controls
    float angleY, angleX;      // rotation around Y and X axes
    float camDist;             // distance from camera to camTarget
    glm::vec3 camTarget;       // point the camera looks at
    int lastMouseX, lastMouseY;
    bool rotating;

    // Lighting / shading toggles
    bool lightingEnabled;
    bool shadingEnabled;

    // Scene graph: a list of Object3D pointers
    std::vector<Object3D*> objects;

    // Index of the currently selected object in `objects` (−1 if none)
    int selectedIndex;
};
