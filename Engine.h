/**
 * @file Engine.h
 * @brief Główna klasa silnika 3D – zarządza cyklem życia aplikacji, renderowaniem i obsługą wejścia.
 */

#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Texture2D.h"

class Object3D;

/**
 * @class Engine
 * @brief Klasa reprezentująca silnik 3D oparty na GLUT i OpenGL.
 *
 * Silnik zarządza inicjalizacją kontekstu graficznego, rysowaniem sceny, obsługą zdarzeń wejściowych
 * oraz przechowywaniem obiektów 3D i tekstur. Umożliwia wybór trybu rzutowania (perspektywa lub ortho),
 * zarządzanie kamerą i ustawieniami okna.
 */
class Engine {
public:
    /** @brief Wskaźnik do instancji singletonu silnika. */
    static Engine* instance;

    /**
     * @brief Konstruktor silnika.
     * @param argc liczba argumentów wiersza poleceń (GLUT)
     * @param argv tablica argumentów wiersza poleceń (GLUT)
     */
    Engine(int argc, char** argv);

    /**
     * @brief Destruktor silnika – zwalnia zasoby.
     */
    ~Engine();

    /**
     * @brief Inicjalizacja GLUT, OpenGL i podstawowych ustawień sceny.
     */
    void Init();

    /**
     * @brief Rozpoczyna główną pętlę programu GLUT.
     */
    void Run();

    /**
     * @brief Czyści zasoby i zamyka aplikację.
     */
    void Cleanup();

    // ===== Konfiguracja okna =====

    /** @brief Ustawia rozdzielczość okna. */
    void SetResolution(int w, int h);

    /** @brief Przełącza tryb pełnoekranowy. */
    void SetFullscreen(bool value);

    /** @brief Ustawia kolor czyszczenia (tło). */
    void SetClearColor(float r, float g, float b);

    /** @brief Ustawia liczbę klatek na sekundę. */
    void SetFPS(int frames);

    // ===== Tryby projekcji =====

    /**
     * @brief Ustawia projekcję perspektywiczną.
     * @param fovDeg kąt widzenia w stopniach
     * @param zn bliska płaszczyzna obcinania
     * @param zf daleka płaszczyzna obcinania
     */
    void SetPerspective(float fovDeg, float zn, float zf);

    /**
     * @brief Ustawia projekcję ortogonalną.
     */
    void SetOrtho(float left, float right, float bottom, float top, float zn, float zf);

    // ===== Callbacki GLUT (statyczne) =====

    static void DisplayCallback();
    static void ReshapeCallback(int w, int h);
    static void KeyboardCallback(unsigned char k, int x, int y);
    static void SpecialCallback(int key, int x, int y);
    static void MouseCallback(int button, int state, int x, int y);
    static void MotionCallback(int x, int y);
    static void TimerCallback(int value);

    /** @brief Kolekcja tekstur ładowanych przy starcie. */
    std::vector<Texture2D*> textures;

private:
    // Kopiowanie niedozwolone
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    // Obsługa zdarzeń (wewnętrzne)
    void Display();
    void Reshape(int w, int h);
    void Keyboard(unsigned char key, int x, int y);
    void Special(int key, int x, int y);
    void Mouse(int button, int state, int x, int y);
    void Motion(int x, int y);
    void OnTimer(int value);

    /** @brief Rysuje nakładkę pomocy na ekranie. */
    void DrawHelpOverlay();

    // ===== Stan okna i renderera =====
    int width, height;       ///< Rozdzielczość okna
    bool fullscreen;         ///< Czy w trybie pełnoekranowym
    int window;              ///< Uchwyt okna GLUT
    bool showHelp;           ///< Czy wyświetlić nakładkę pomocy

    // ===== Synchronizacja klatek =====
    int fps;                 ///< Liczba FPS

    // ===== Kolor tła =====
    glm::vec3 clearColor;    ///< Kolor czyszczenia bufora

    // ===== Tryby projekcji =====
    enum class ProjectionMode { Perspective, Ortho };
    ProjectionMode projMode;
    float fov, zNear, zFar;
    float orthoLeft, orthoRight, orthoBottom, orthoTop;

    // ===== Sterowanie kamerą =====
    float angleY, angleX;           ///< Obroty wokół osi Y i X
    float camDist;                  ///< Odległość od celu kamery
    glm::vec3 camTarget;            ///< Punkt, na który patrzy kamera
    int lastMouseX, lastMouseY;     ///< Pozycja myszy
    bool rotating;                  ///< Czy użytkownik obraca scenę

    // ===== Ustawienia oświetlenia =====
    bool lightingEnabled;
    bool shadingEnabled;

    // ===== Scena 3D =====
    std::vector<Object3D*> objects; ///< Lista obiektów w scenie

    int selectedIndex; ///< Indeks aktualnie zaznaczonego obiektu (lub −1 jeśli brak)
};
