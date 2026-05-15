#pragma once
#include "glm/glm.hpp"

using glm::vec2, glm::ivec2;

class Cell{
    public:

    float pressure;
    vec2 position;
    int size;
    float divergence;
    bool solid = false;
   
    Cell() = default;

    Cell(vec2 pos, float pressure, int size) : position(pos), pressure(pressure), size(size) {}
};