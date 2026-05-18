#pragma once
#include "Cell.hpp"
#include <iostream>
#include <algorithm>
#include <raylib.h>
#include <thread>
#include <vector>

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

    Image cellimage;
    Color* cellimagedata;
    Texture2D celltexture;

    Vector2 lastMousePos = {-1, -1};
    bool shoulddrawarrow = false;

    float brushsize;
    float drawbrushtimer = 0;

    int selectedbrush = 1;
    int selectedoverlay = 6;

    float* smoke;
    float* smoketemp;

    Cells(ivec2 size, float cellsize);
    void Draw();
    void Update(float dt, Vector2 mousePos);
    void UpdatePressureWrapper(float);
    void UpdatePressure(float, int, int);
    void UpdatePressureMid(float, int, int, bool);
    void UpdatePressureEdge(float, bool);
    float GetDivergence(ivec2);
    vec2 GetVelocity(ivec2);
    void UpdateVelocities(float deltaTime);
    void ApplyPressure(float deltaTime);
    vec2 GetVelocityAtPosition(vec2 pos);
    void SetVelocities();
    void AdvectSmoke(float);
    float SampleSmoke(vec2);
    void SmokeMouse(Vector2);
    void MouseVelocityChange(Vector2 mousePos);
    void MouseSolid(Vector2 mousePos);
    void DrawMouseCircle();
    void ChangeBrush();
    ~Cells();
};