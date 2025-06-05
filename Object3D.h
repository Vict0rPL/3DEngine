// Object3D.h
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Object3D {
public:
    Object3D()
        : position(0.0f), rotation(0.0f), scale(1.0f),
        selected(false) 
    {
    }

    virtual ~Object3D() {}

    // Transform setters
    void SetPosition(const glm::vec3& pos) { position = pos; }
    void SetRotation(const glm::vec3& rot) { rotation = rot; }
    void SetScale(const glm::vec3& scl) { scale = scl; }

    // Model matrix
    glm::mat4 GetModelMatrix() const {
        glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0));
        glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0));
        glm::mat4 Rz = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1));
        glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
        return T * Rz * Ry * Rx * S;
    }

    glm::vec3 GetPosition() const { return position; }

    virtual void Draw() = 0;

    // selection API 
    void SetSelected(bool s) { selected = s; }
    bool IsSelected() const { return selected; }

protected:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    bool selected;
};
