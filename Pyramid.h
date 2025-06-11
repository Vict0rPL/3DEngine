/**
 * @file Pyramid.h
 * @brief Definicja klasy Pyramid – obiektu 3D w kształcie ostrosłupa.
 */

#pragma once

#include "Object3D.h"

 /**
  * @class Pyramid
  * @brief Klasa reprezentująca ostrosłup 3D (piramidę).
  *
  * Dziedziczy po Object3D i implementuje metodę Draw(), która renderuje obiekt jako ostrosłup.
  */
class Pyramid : public Object3D {
public:
    /**
     * @brief Rysuje ostrosłup w scenie 3D.
     *
     * Implementacja funkcji rysowania wykorzystująca aktualną pozycję, rotację, skalę
     * oraz opcjonalnie teksturowanie i stan zaznaczenia.
     */
    void Draw() override;
};
