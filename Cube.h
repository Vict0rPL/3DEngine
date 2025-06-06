// Cube.h
#pragma once
#include "Object3D.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

class Cube : public Object3D {
public:
    Cube() = default;
    virtual ~Cube() = default;

    virtual void Draw() override;
};