/**
 * @file Engine.cpp
 * @brief Implementacja klasy Engine odpowiedzialnej za inicjalizację, zarządzanie i renderowanie sceny 3D.
 *
 * Zawiera definicje metod do obsługi okna, ustawień kamery, obsługi tekstur, oświetlenia oraz interakcji użytkownika.
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Engine.h"
#include "Cube.h"
#include "Pyramid.h"
#include "Sphere.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

 /**
  * @brief Statyczny wskaźnik singletona klasy Engine.
  */
Engine* Engine::instance = nullptr;

/**
 * @brief Konstruktor klasy Engine.
 * @param argc Liczba argumentów przekazanych do programu.
 * @param argv Tablica argumentów przekazanych do programu.
 *
 * Inicjalizuje parametry silnika oraz ustawia wskaźnik singletona.
 */
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
    selectedIndex(-1),
    showHelp(false)
{
    // Inicjalizacja GLUT
    glutInit(&argc, argv);

    // Ustaw wskaźnik singletona
    instance = this;
}

/**
 * @brief Destruktor klasy Engine.
 *
 * Usuwa wszystkie załadowane tekstury oraz obiekty sceny.
 */
Engine::~Engine() {
    // Usuwanie tekstur
    for (auto tex : textures) {
        if (tex) {
            tex->Delete();
            delete tex;
        }
    }
    textures.clear();

    // Usuwanie obiektów sceny
    for (auto obj : objects) {
        delete obj;
    }
    objects.clear();
}

/**
 * @brief Inicjalizuje silnik, tworzy okno i ustawia OpenGL.
 *
 * Tworzy okno GLUT, ustawia tryb wyświetlania, konfiguruje oświetlenie, ładuje tekstury oraz rejestruje callbacki GLUT.
 */
void Engine::Init() {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    window = glutCreateWindow("3D Engine");

    GLenum glewErr = glewInit();
    if (glewErr != GLEW_OK) {
        std::cerr << "GLEW initialization failed: "
            << glewGetErrorString(glewErr) << std::endl;
        exit(1);
    }

    if (fullscreen) {
        glutFullScreen();
    }

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat position[] = { 10.0f, 10.0f, 10.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    // Ładowanie tekstur
    textures.push_back(new Texture2D("brick.png"));
    textures.push_back(new Texture2D("wood.png"));
    textures.push_back(new Texture2D("avocado.png"));
    textures.push_back(new Texture2D("burgers.png"));
    textures.push_back(new Texture2D("sky.png"));
    textures.push_back(new Texture2D("planet.png"));
    textures.push_back(new Texture2D("holo.png"));
    textures.push_back(new Texture2D("hoth.png"));
    textures.push_back(new Texture2D("moon.png"));
    textures.push_back(new Texture2D("holo.png"));
    textures.push_back(new Texture2D("deathstar.png"));

    // Tworzenie obiektu startowego i ustawienie selekcji
    objects.push_back(new Cube());
    selectedIndex = 0;

    // Rejestracja callbacków GLUT
    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutSpecialFunc(SpecialCallback);
    glutMouseFunc(MouseCallback);
    glutMotionFunc(MotionCallback);
    glutTimerFunc(0, TimerCallback, 0);
}

/**
 * @brief Uruchamia główną pętlę zdarzeń GLUT.
 */
void Engine::Run() {
    glutMainLoop();
}

/**
 * @brief Czyści zasoby i niszczy okno.
 */
void Engine::Cleanup() {
    std::cout << "Cleaning up...\n";
    if (window != 0) {
        glutDestroyWindow(window);
        window = 0;
    }
}

/**
 * @brief Ustawia rozdzielczość okna.
 * @param w Szerokość okna w pikselach.
 * @param h Wysokość okna w pikselach.
 */
void Engine::SetResolution(int w, int h) {
    width = w;
    height = h;
}

/**
 * @brief Ustawia tryb pełnoekranowy.
 * @param value true - włącz pełny ekran, false - tryb okna.
 */
void Engine::SetFullscreen(bool value) {
    fullscreen = value;
}

/**
 * @brief Ustawia kolor czyszczenia bufora kolorów.
 * @param r Składowa czerwona (0.0 - 1.0).
 * @param g Składowa zielona (0.0 - 1.0).
 * @param b Składowa niebieska (0.0 - 1.0).
 */
void Engine::SetClearColor(float r, float g, float b) {
    clearColor = glm::vec3(r, g, b);
    glClearColor(r, g, b, 1.0f);
}

/**
 * @brief Ustawia liczbę klatek na sekundę.
 * @param frames Liczba klatek na sekundę.
 */
void Engine::SetFPS(int frames) {
    fps = frames;
}


/**
 * @brief Ustawia projekcję perspektywiczną.
 * @param fovDeg Kąt widzenia (field of view) w stopniach.
 * @param zn Odległość najbliższej płaszczyzny przycinania (near plane).
 * @param zf Odległość najdalszej płaszczyzny przycinania (far plane).
 *
 * Ustawia tryb projekcji na perspektywiczną oraz zapisuje parametry projekcji.
 */
void Engine::SetPerspective(float fovDeg, float zn, float zf) {
    projMode = ProjectionMode::Perspective;
    fov = fovDeg;
    zNear = zn;
    zFar = zf;
}

/**
 * @brief Ustawia projekcję ortograficzną.
 * @param left Lewa granica widoku.
 * @param right Prawa granica widoku.
 * @param bottom Dolna granica widoku.
 * @param top Górna granica widoku.
 * @param zn Odległość najbliższej płaszczyzny przycinania (near plane).
 * @param zf Odległość najdalszej płaszczyzny przycinania (far plane).
 *
 * Ustawia tryb projekcji na ortograficzną oraz zapisuje parametry projekcji.
 */
void Engine::SetOrtho(float left, float right, float bottom, float top, float zn, float zf) {
    projMode = ProjectionMode::Ortho;
    orthoLeft = left;
    orthoRight = right;
    orthoBottom = bottom;
    orthoTop = top;
    zNear = zn;
    zFar = zf;
}

/**
 * @brief Funkcja wywoływana podczas renderowania sceny.
 *
 * Czyści bufor kolorów i głębokości, ustawia macierz widoku (kamery), rysuje wszystkie obiekty
 * na scenie oraz wyświetla pomoc, jeśli jest włączona.
 * Na koniec zamienia bufor wyświetlania.
 */
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

    // Mark exactly one object as selected
    for (int i = 0; i < (int)objects.size(); ++i) {
        objects[i]->SetSelected(i == selectedIndex);
    }

    // Draw all objects
    for (auto obj : objects) {
        obj->Draw();
    }

    if (showHelp) {
        DrawHelpOverlay();
    }

    // Swap buffers
    glutSwapBuffers();
}

/**
 * @brief Funkcja wywoływana przy zmianie rozmiaru okna.
 * @param w Nowa szerokość okna.
 * @param h Nowa wysokość okna.
 *
 * Aktualizuje viewport i macierz projekcji zgodnie z aktualnym trybem projekcji (perspektywa lub orto).
 */
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



/**
 * @brief Obsługa zdarzeń klawiatury (klawisze ASCII).
 * @param key Wciśnięty klawisz.
 * @param x Pozycja kursora myszy w chwili wciśnięcia (nieużywane).
 * @param y Pozycja kursora myszy w chwili wciśnięcia (nieużywane).
 *
 * Funkcja reaguje na wciśnięcia klawiszy sterujących:
 * - ESC - wyjście z programu,
 * - Tab - zmiana wybranego obiektu,
 * - P/O - zmiana trybu projekcji (perspektywa/ortho),
 * - L/K - włączanie/wyłączanie oświetlenia i modelu cieniowania,
 * - WASDQE - przesuwanie kamery,
 * - M/N - skalowanie wybranego obiektu,
 * - Z/X, C/V, F/G - obrót wybranego obiektu wokół osi X, Y, Z,
 * - T/R/Y - włączanie/wyłączanie tekstur, zmiana tekstury,
 * - H - pokaz/ukryj pomoc,
 * - 1/2/3 - dodanie nowego obiektu (kostka, piramida, kula),
 * - 9/0 - przesunięcie wybranego obiektu wzdłuż osi Z.
 *
 * Po przetworzeniu zdarzenia następuje wywołanie funkcji ponownego renderowania.
 */
void Engine::Keyboard(unsigned char key, int, int) {
    const float moveStep = 0.1f;
    const float rotStep = 0.1f;    // ~0.1 rad ≈ 5.7°

    // If no object is selected, we ignore T/R/Y
    Object3D* selObj = nullptr;
    if (selectedIndex >= 0 && selectedIndex < (int)objects.size()) {
        selObj = objects[selectedIndex];
    }

    switch (key) {
    case 27: // ESC
        exit(0);
        break;

    case '\t': // (Tab) cycle selection
        if (!objects.empty()) {
            selectedIndex = (selectedIndex + 1) % (int)objects.size();
            std::cout << "Selected object index = " << selectedIndex << "\n";
        }
        break;
    case 'P':
    case 'p': // Switch to perspective
        SetPerspective(fov, zNear, zFar);
        Reshape(width, height);
        break;
    case 'O':
    case 'o': // Switch to orthographic
        SetOrtho(orthoLeft, orthoRight, orthoBottom, orthoTop, zNear, zFar);
        Reshape(width, height);
        break;
    case 'L':
    case 'l': // Toggle lighting
        lightingEnabled = !lightingEnabled;
        if (lightingEnabled) glEnable(GL_LIGHTING);
        else                glDisable(GL_LIGHTING);
        break;
    case 'K':
    case 'k': // Toggle shading model
        shadingEnabled = !shadingEnabled;
        if (shadingEnabled)
            glShadeModel(GL_SMOOTH);
        else
            glShadeModel(GL_FLAT);
        break;
    case 'W':
    case 'w': // Pan camera up
        camTarget.y += moveStep;
        break;
    case 'S':
    case 's': // Pan camera down
        camTarget.y -= moveStep;
        break;
    case 'A':
    case 'a': // Pan camera left
        camTarget.x -= moveStep;
        break;
    case 'D':
    case 'd': // Pan camera right
        camTarget.x += moveStep;
        break;
    case 'Q':
    case 'q': // Move camera forward
        camTarget.z += moveStep;
        break;
    case 'E':
    case 'e': // Move camera backward
        camTarget.z -= moveStep;
        break;
    case 'M':
    case 'm': { // scale up by 10%
        if (selectedIndex >= 0 && selectedIndex < (int)objects.size()) {
            Object3D* selObj = objects[selectedIndex];
            glm::vec3 scl = selObj->GetScale();
            scl *= 1.1f;  // increase each component by 10%
            selObj->SetScale(scl);
        }
        break;
    }
    case 'N':
    case 'n': { // scale down by 10%
        if (selectedIndex >= 0 && selectedIndex < (int)objects.size()) {
            Object3D* selObj = objects[selectedIndex];
            glm::vec3 scl = selObj->GetScale();
            scl *= 0.9f;  // decrease each component by 10%
            selObj->SetScale(scl);
        }
        break;
    }
     //Rotate selected object along X, Y, Z with z/x, c/v, f/g
    case 'Z':
    case 'z': {
        if (selectedIndex >= 0 && selectedIndex < (int)objects.size()) {
            Object3D* selObj = objects[selectedIndex];
            glm::vec3 rot = selObj->GetRotation();
            rot.x += rotStep;       // rotate +0.1 rad about X
            selObj->SetRotation(rot);
        }
        break;
    }
    case 'X':
    case 'x': {
        if (selectedIndex >= 0 && selectedIndex < (int)objects.size()) {
            Object3D* selObj = objects[selectedIndex];
            glm::vec3 rot = selObj->GetRotation();
            rot.x -= rotStep;       // rotate –0.1 rad about X
            selObj->SetRotation(rot);
        }
        break;
    }
    case 'C':
    case 'c': {
        if (selectedIndex >= 0 && selectedIndex < (int)objects.size()) {
            Object3D* selObj = objects[selectedIndex];
            glm::vec3 rot = selObj->GetRotation();
            rot.y += rotStep;       // rotate +0.1 rad about Y
            selObj->SetRotation(rot);
        }
        break;
    }
    case 'V':
    case 'v': {
        if (selectedIndex >= 0 && selectedIndex < (int)objects.size()) {
            Object3D* selObj = objects[selectedIndex];
            glm::vec3 rot = selObj->GetRotation();
            rot.y -= rotStep;       // rotate –0.1 rad about Y
            selObj->SetRotation(rot);
        }
        break;
    }
    case 'F':
    case 'f': {
        if (selectedIndex >= 0 && selectedIndex < (int)objects.size()) {
            Object3D* selObj = objects[selectedIndex];
            glm::vec3 rot = selObj->GetRotation();
            rot.z += rotStep;       // rotate +0.1 rad about Z
            selObj->SetRotation(rot);
        }
        break;
    }
    case 'G':
    case 'g': {
        if (selectedIndex >= 0 && selectedIndex < (int)objects.size()) {
            Object3D* selObj = objects[selectedIndex];
            glm::vec3 rot = selObj->GetRotation();
            rot.z -= rotStep;       // rotate –0.1 rad about Z
            selObj->SetRotation(rot);
        }
        break;
    }

    // Toggle texturing on/off
    case 'T':   // Toggle textured flag for selected object
    case 't': {
        if (selObj) {
            bool current = selObj->IsTextured();
            selObj->SetTextured(!current);
            std::cout << "Object " << selectedIndex
                << (selObj->IsTextured() ? ": Texturing ON\n"
                    : ": Texturing OFF\n");
        }
        break;
    }
    case 'R':   // go to previous texture index
    case 'r': {
        if (selObj && !textures.empty()) {
            int idx = selObj->GetTexIndex();
            idx = (idx - 1 + (int)textures.size()) % (int)textures.size();
            selObj->SetTexIndex(idx);
            std::cout << "Object " << selectedIndex
                << ": now using texture #" << idx << "\n";
        }
        break;
    }
    case 'Y':  // go to next texture index
    case 'y': {
        if (selObj && !textures.empty()) {
            int idx = selObj->GetTexIndex();
            idx = (idx + 1) % (int)textures.size();
            selObj->SetTexIndex(idx);
            std::cout << "Object " << selectedIndex
                << ": now using texture #" << idx << "\n";
        }
        break;
    }
    case 'H':
    case 'h':
        showHelp = !showHelp;
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
    case '9': {
        if (selectedIndex >= 0 && selectedIndex < (int)objects.size()) {
            Object3D* selObj = objects[selectedIndex];
            glm::vec3 pos = selObj->GetPosition();
            pos.z -= moveStep;      // move down (- Z)
            selObj->SetPosition(pos);
        }
        break;
    }
    case '0': {
        if (selectedIndex >= 0 && selectedIndex < (int)objects.size()) {
            Object3D* selObj = objects[selectedIndex];
            glm::vec3 pos = selObj->GetPosition();
            pos.z += moveStep;      // move up (+ Z)
            selObj->SetPosition(pos);
        }
        break;
    }

    default:
        break;
    }
    
    // Request redisplay
    glutPostRedisplay();
}

/**
 * @brief Obsługa klawiszy specjalnych (strzałki itp.).
 * @param key Kod wciśniętego klawisza specjalnego (GLUT_KEY_*).
 * @param x Pozycja kursora myszy w chwili wciśnięcia (nieużywane).
 * @param y Pozycja kursora myszy w chwili wciśnięcia (nieużywane).
 *
 * Funkcja przesuwa wybrany obiekt w płaszczyźnie XY w zależności od strzałek:
 * - Strzałka w lewo/prawo przesuwa wzdłuż osi X,
 * - Strzałka w górę/dół przesuwa wzdłuż osi Y.
 *
 * Po przetworzeniu zdarzenia następuje wywołanie funkcji ponownego renderowania.
 */
void Engine::Special(int key, int /*x*/, int /*y*/) {
    // How much we move per keypress
    const float moveStep = 0.1f;

    // If no object is selected, do nothing
    if (selectedIndex >= 0 && selectedIndex < (int)objects.size()) {
        // Get pointer to currently selected Object3D
        Object3D* selObj = objects[selectedIndex];
        // Read its current position
        glm::vec3 pos = selObj->GetPosition();

        switch (key) {
        case GLUT_KEY_LEFT:
            pos.x -= moveStep;
            break;
        case GLUT_KEY_RIGHT:
            pos.x += moveStep;
            break;
        case GLUT_KEY_UP:
            pos.y += moveStep;
            break;
        case GLUT_KEY_DOWN:
            pos.y -= moveStep;
            break;
        default:
            break;
        }

        // Set the new position back on the object
        selObj->SetPosition(pos);
    }

    // redisplay after handling special keys
    glutPostRedisplay();
}



/**
 * @brief Obsługa zdarzeń myszy - kliknięcia i przewijania.
 * @param button Naciśnięty przycisk myszy lub scroll (GLUT_LEFT_BUTTON, 3 = scroll up, 4 = scroll down).
 * @param state Stan przycisku (GLUT_DOWN lub GLUT_UP).
 * @param x Pozycja kursora myszy w osi X.
 * @param y Pozycja kursora myszy w osi Y.
 *
 * Funkcja odpowiada za rozpoczęcie i zakończenie obracania kamery (kliknięcie lewego przycisku myszy)
 * oraz za zoomowanie kamery przy użyciu scrolla.
 * Po każdej zmianie wywołuje ponowne przerysowanie sceny.
 */
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


/**
 * @brief Obsługa ruchu myszy (przeciąganie) podczas obracania kamery.
 * @param x Aktualna pozycja kursora myszy w osi X.
 * @param y Aktualna pozycja kursora myszy w osi Y.
 *
 * Jeśli kamera jest w trybie obracania (rotating == true),
 * to funkcja aktualizuje kąty obrotu kamery na podstawie ruchu myszy
 * i prosi o przerysowanie sceny.
 */
void Engine::Motion(int x, int y) {
    if (rotating) {
        angleY += (x - lastMouseX) * 0.005f;
        angleX -= (y - lastMouseY) * 0.005f;
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

/**
 * @brief Rysuje nakładkę tekstową z pomocą (instrukcjami obsługi) w trybie ortograficznym.
 *
 * Funkcja ustawia odpowiednią macierz projekcji i modelu,
 * wyłącza oświetlenie i test głębokości,
 * a następnie renderuje linie tekstu z instrukcjami obsługi klawiatury i myszy.
 * Po zakończeniu przywraca poprzedni stan OpenGL.
 */
void Engine::DrawHelpOverlay() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);

    // Wyłącz to, co przeszkadza w nakładce tekstowej
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Ustaw rzutowanie ortograficzne w tym samym współczynniku, co okno
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Kolor tekstu i pozycja początkowa
    glColor3f(1.0f, 1.0f, 1.0f);
    const int lineHeight = 18;
    int y = height - lineHeight; // zaczynamy od góry

    // Tablica linii pomocy
    const char* helpLines[] = {
        "===== Help: Keyboard Commands =====",
        "ESC           - Exit",
        "TAB           - Cycle selected object",
        "W/S/A/D/Q/E   - Pan camera target up/down/left/right/forward/back",
        "Arrow Keys    - Move selected object in X/Y",
        "9 / 0         - Move selected object along Z axis",
        "Z / X         - Rotate selected object around X axis",
        "C / V         - Rotate selected object around Y axis",
        "F / G         - Rotate selected object around Z axis",
        "M / N         - Scale selected object up/down",
        "T             - Toggle texturing for selected object",
        "R / Y         - Previous / Next texture",
        "L             - Toggle lighting on/off",
        "K             - Toggle shading on/off",
        "P / O         - Perspective / Orthographic projection",
        "1             - Add Cube",
        "2             - Add Pyramid",
        "3             - Add Sphere",
        "Mouse Drag    - Rotate camera",
        "Mouse Wheel   - Zoom in/out",
        "H             - Toggle this help overlay",
        "===================================="
    };
    const int linesCount = sizeof(helpLines) / sizeof(helpLines[0]);

    // Narysuj kolejne linie przy użyciu GLUT bitmap font
    for (int i = 0; i < linesCount; ++i) {
        glRasterPos2i(10, y - i * lineHeight);
        const char* line = helpLines[i];
        while (*line) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *line++);
        }
    }

    // Przywróć poprzedni stan
    glPopAttrib();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}



/**
 * @brief Handler timera wywoływany po upływie czasu.
 * @param value Wartość przekazywana do timera (nieużywana).
 *
 * Funkcja wywołuje ponowne przerysowanie sceny i ustawia kolejny wywołanie timera
 * zgodnie z wartością fps (klatek na sekundę).
 */
void Engine::OnTimer(int value) {
    glutPostRedisplay();
    // Schedule the next timer callback
    glutTimerFunc(1000 / fps, TimerCallback, 0);
}


/**
 * @brief Statyczna funkcja callback timera GLUT.
 * @param value Wartość przekazywana do timera.
 *
 * Funkcja przekazuje wywołanie do metody instancji klasy Engine.
 */
void Engine::TimerCallback(int value) {
    // Call the renamed instance method
    instance->OnTimer(value);
}


/**
 * @brief Statyczna funkcja callback GLUT do wyświetlania sceny.
 *
 * Wywołuje metodę Display() instancji klasy Engine.
 */
void Engine::DisplayCallback() {
    instance->Display();
}

/**
 * @brief Statyczna funkcja callback GLUT obsługująca zmianę rozmiaru okna.
 * @param w Nowa szerokość okna.
 * @param h Nowa wysokość okna.
 *
 * Przekazuje wywołanie do metody Reshape() instancji klasy Engine.
 */
void Engine::ReshapeCallback(int w, int h) {
    instance->Reshape(w, h);
}

/**
 * @brief Statyczna funkcja callback GLUT obsługująca zdarzenia klawiatury.
 * @param k Naciśnięty klawisz.
 * @param x Pozycja kursora myszy w osi X (nieużywana).
 * @param y Pozycja kursora myszy w osi Y (nieużywana).
 *
 * Przekazuje wywołanie do metody Keyboard() instancji klasy Engine.
 */
void Engine::KeyboardCallback(unsigned char k, int x, int y) {
    instance->Keyboard(k, x, y);
}

/**
 * @brief Statyczna funkcja callback GLUT obsługująca klawisze specjalne.
 * @param key Kod klawisza specjalnego (np. strzałki).
 * @param x Pozycja kursora myszy w osi X (nieużywana).
 * @param y Pozycja kursora myszy w osi Y (nieużywana).
 *
 * Przekazuje wywołanie do metody Special() instancji klasy Engine.
 */
void Engine::SpecialCallback(int key, int x, int y) {
    instance->Special(key, x, y);
}

/**
 * @brief Statyczna funkcja callback GLUT obsługująca przyciski myszy.
 * @param button Naciśnięty przycisk myszy.
 * @param state Stan przycisku (GLUT_DOWN lub GLUT_UP).
 * @param x Pozycja kursora myszy w osi X.
 * @param y Pozycja kursora myszy w osi Y.
 *
 * Przekazuje wywołanie do metody Mouse() instancji klasy Engine.
 */
void Engine::MouseCallback(int button, int state, int x, int y) {
    instance->Mouse(button, state, x, y);
}

/**
 * @brief Statyczna funkcja callback GLUT obsługująca ruch myszy.
 * @param x Pozycja kursora myszy w osi X.
 * @param y Pozycja kursora myszy w osi Y.
 *
 * Przekazuje wywołanie do metody Motion() instancji klasy Engine.
 */
void Engine::MotionCallback(int x, int y) {
    instance->Motion(x, y);
}
