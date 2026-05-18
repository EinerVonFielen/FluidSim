#pragma once
#include "glm/glm.hpp"

using glm::vec2, glm::ivec2;

class Cell{
    public:

    float pressure;
    bool notsolid = true;
   
    Cell() = default;

    Cell(float pressure) : pressure(pressure) {}
};