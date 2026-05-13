#pragma once
#include "Cell.hpp"
#include <iostream>
#include <raylib.h>

constexpr Color BLUEGRAY{45, 58, 69, 255};
constexpr Color GRAY1{44, 48, 51, 255};
constexpr Color GRAY2{35, 38, 41, 255};

class Cells{

    public:

    ivec2 size;
    Cell* cells;
    int cellcount;
    float cellsize;
    float* velocitiesX;
    float* velocitiesY;

    Cells(ivec2 size, float cellsize);
    void Draw();
    void Update(float dt);
    void UpdatePressure(float deltaTime);
    void UpdateVelocities(float deltaTime);
    void SetVelocities();
    ~Cells();
};