#include <raylib.h>
#include <iostream>
#include "glm/glm.hpp"

using glm::vec2;

class Scene{

    public:

    Camera2D camera;
    vec2 windowSize;

    Scene() = default;
    Scene(vec2 size) : windowSize(size) { camera = { Vector2{ windowSize.x/2, windowSize.y/2}, { 0.0f, 0.0f }, 0.0f, 1.0f }; }
    void CameraUpdate();
    void Draw();
};