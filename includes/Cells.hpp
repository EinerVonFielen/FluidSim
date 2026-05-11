#pragma once
#include "Cell.hpp"
#include <iostream>
#include <raylib.h>

#define BLUEGRAY {45, 58, 69, 255}
#define GRAY1 {44, 48, 51, 255}
#define GRAY2 {35, 38, 41, 255} 

class Cells{

    public:

    vec2 size;
    Cell* cells;
    int cellcount;
    float cellsize;
    float* velocitiesX;
    float* velocitiesY;

    Cells(vec2 size, float cellsize);
    void Draw();
    void Update();
    ~Cells();
};