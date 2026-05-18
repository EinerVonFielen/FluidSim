#pragma once
#include "Cell.hpp"
#include <iostream>
#include <algorithm>
#include <raylib.h>
#include <execution>

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
    float* newvelocitiesX;
    float* newvelocitiesY;
    
    std::vector<std::pair<uint32_t, uint32_t>> m_CheckerEven, m_CheckerOdd;

    Image cellimage;
    Color* cellimagedata;
    Texture2D celltexture;

    Vector2 lastMousePos = {-1, -1};
    bool shoulddrawarrow = false;

    float brushsize;
    float drawbrushtimer = 0;

    int selectedbrush = 1;
    int selectedoverlay = 6;

    Cells(ivec2 size, float cellsize);
    void Draw();
    void Update(float dt, Vector2 mousePos);
    void UpdatePressureWrapper(float);
    void UpdatePressure(float, uint32_t, uint32_t);
    void UpdateDivergence();
    void UpdateVelocities(float deltaTime);
    void ApplyPressure(float deltaTime);
    vec2 GetVelocityAtPosition(vec2 pos);
    void SetVelocities();
    void MouseVelocityChange(Vector2 mousePos);
    void MouseSolid(Vector2 mousePos);
    void DrawMouseCircle();
    void ChangeBrush();
    ~Cells();
};