/**
 * @file Sphere.h
 * @brief Definicja klasy Sphere – obiektu 3D w kształcie kuli.
 */

#pragma once

#include "Object3D.h"

 /**
  * @class Sphere
  * @brief Klasa reprezentująca kulę 3D.
  *
  * Dziedziczy po Object3D i implementuje metodę Draw(), która renderuje obiekt jako sferę.
  */
class Sphere : public Object3D {
public:
    /**
     * @brief Rysuje kulę w scenie 3D.
     *
     * Implementacja metody rysującej kulę z uwzględnieniem transformacji i ewentualnego teksturowania.
     */
    void Draw() override;
};
