#pragma once
#include "glm/glm.hpp"

using glm::vec2;

class Cell{
    public:

    float pressure;
    vec2 position;
    int size;
   
    Cell() = default;

    Cell(vec2 pos, float pressure, int size) : position(pos), pressure(pressure), size(size) {}
};