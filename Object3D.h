/**
 * @file Object3D.h
 * @brief Klasa bazowa dla wszystkich obiektów 3D – zawiera informacje o transformacjach i renderowaniu.
 */

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

 /**
  * @class Object3D
  * @brief Abstrakcyjna klasa bazowa reprezentująca obiekt 3D w scenie.
  *
  * Zawiera informacje o pozycji, rotacji, skali, wyborze oraz obsłudze tekstur.
  * Udostępnia interfejs do rysowania obiektów (`Draw()`), który musi być implementowany w klasach pochodnych.
  */
class Object3D {
public:
    /**
     * @brief Konstruktor domyślny.
     *
     * Inicjalizuje pozycję na (0,0,0), skalę na (1,1,1), brak tekstury i brak zaznaczenia.
     */
    Object3D()
        : position(0.0f),
        rotation(0.0f),
        scale(1.0f),
        selected(false),
        textured(false),
        texIndex(0)
    {
    }

    /**
     * @brief Wirtualny destruktor.
     */
    virtual ~Object3D() {}

    // ===== Transformacje =====

    /**
     * @brief Ustawia pozycję obiektu.
     * @param pos Nowa pozycja jako wektor 3D
     */
    void SetPosition(const glm::vec3& pos) { position = pos; }

    /**
     * @brief Ustawia rotację obiektu (w radianach).
     * @param rot Rotacja wokół osi X, Y i Z
     */
    void SetRotation(const glm::vec3& rot) { rotation = rot; }

    /**
     * @brief Ustawia skalę obiektu.
     * @param scl Wektor skalowania w osiach X, Y, Z
     */
    void SetScale(const glm::vec3& scl) { scale = scl; }

    /**
     * @brief Zwraca macierz modelu dla obiektu (transformacja lokalna).
     *
     * Macierz zawiera translację, rotację (Rx, Ry, Rz) oraz skalowanie.
     * @return Macierz modelu typu `glm::mat4`
     */
    glm::mat4 GetModelMatrix() const {
        glm::mat4 T = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0));
        glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0));
        glm::mat4 Rz = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1));
        glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
        return T * Rz * Ry * Rx * S;
    }

    /**
     * @brief Zwraca aktualną pozycję obiektu.
     */
    glm::vec3 GetPosition() const { return position; }

    /**
     * @brief Zwraca aktualną skalę obiektu.
     */
    glm::vec3 GetScale() const { return scale; }

    /**
     * @brief Zwraca aktualną rotację obiektu.
     */
    glm::vec3 GetRotation() const { return rotation; }

    // ===== Rysowanie =====

    /**
     * @brief Funkcja czysto wirtualna – rysuje obiekt w scenie.
     *
     * Musi być zaimplementowana w klasach dziedziczących.
     */
    virtual void Draw() = 0;

    // ===== Zaznaczenie =====

    /**
     * @brief Ustawia stan zaznaczenia obiektu.
     */
    void SetSelected(bool s) { selected = s; }

    /**
     * @brief Sprawdza, czy obiekt jest zaznaczony.
     */
    bool IsSelected() const { return selected; }

    // ===== Teksturowanie =====

    /**
     * @brief Ustawia, czy obiekt ma być teksturowany.
     */
    void SetTextured(bool on) { textured = on; }

    /**
     * @brief Sprawdza, czy obiekt ma aktywną teksturę.
     */
    bool IsTextured() const { return textured; }

    /**
     * @brief Ustawia indeks tekstury przypisanej do obiektu.
     */
    void SetTexIndex(int idx) { texIndex = idx; }

    /**
     * @brief Zwraca indeks tekstury przypisanej do obiektu.
     */
    int GetTexIndex() const { return texIndex; }

protected:
    glm::vec3 position;  ///< Pozycja obiektu
    glm::vec3 rotation;  ///< Rotacja w radianach
    glm::vec3 scale;     ///< Skala

    bool selected;       ///< Czy obiekt jest zaznaczony

    bool textured;       ///< Czy obiekt ma teksturę
    int texIndex;        ///< Indeks tekstury
};
