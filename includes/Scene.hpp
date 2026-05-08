#include <raylib.h>
#include <iostream>
#include "glm/glm.hpp"

using glm::vec2;

class Scene{

    public:

    Camera2D camera;
    vec2 windowSize;


    Scene(vec2 size) : windowSize(size) { }
};