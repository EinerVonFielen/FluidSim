#pragma once
#include "Cell.hpp"
#include "glm/glm.hpp"
#include <glm/vec2.hpp>

using glm::vec2;

class Cells{

    public:

    vec2 size;
    Cell* cells;
    int cellcount;
    int startHeight;
    int startWidth;
    float* velocitiesX;
    float* velocitiesY;

    Cells(vec2 size, int startHeight, int startWidth);
    void Draw();
    ~Cells();

};