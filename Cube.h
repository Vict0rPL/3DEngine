/**
 * @file Cube.h
 * @brief Deklaracja klasy Cube – reprezentuje sześcian 3D dziedziczący po Object3D.
 */

#pragma once

#include "Object3D.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

 /**
  * @class Cube
  * @brief Klasa reprezentująca sześcian w przestrzeni 3D.
  *
  * Dziedziczy po klasie Object3D i implementuje metodę Draw(),
  * umożliwiającą rysowanie sześcianu przy użyciu OpenGL.
  */
class Cube : public Object3D {
public:
    /**
     * @brief Domyślny konstruktor klasy Cube.
     *
     * Tworzy pusty obiekt sześcianu. Parametry geometryczne są ustalane później.
     */
    Cube() = default;

    /**
     * @brief Wirtualny destruktor klasy Cube.
     *
     * Zapewnia poprawne czyszczenie zasobów w przypadku dziedziczenia.
     */
    virtual ~Cube() = default;

    /**
     * @brief Rysuje sześcian w aktualnej scenie 3D.
     *
     * Funkcja nadpisuje metodę wirtualną z klasy Object3D.
     * Wykorzystuje OpenGL i bibliotekę FreeGLUT do renderowania obiektu.
     */
    virtual void Draw() override;
};
