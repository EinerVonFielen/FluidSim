


#pragma once
#include <glm/vec2.hpp>
#include "glm/glm.hpp"
#include <raylib.h>
#include <iostream>
class Cell{
    public:
    float density;
    glm::vec2 position;
    int startHeight;
    int startWidth;
    int cellcount;
   
    Cell() = default;
    Cell(int x, int y, float density, int startHeight, int startWidth, int cellcount){
        position = glm::vec2(x,y);
        this->density = density;
        this->startHeight = startHeight;
        this->startWidth = startWidth;
        this->cellcount = cellcount;
    }


    void Draw(){
        DrawRectangle(position.x/startHeight, position.y/startWidth, startHeight/cellcount, startWidth/cellcount, RED);
    }   
    void updateDensity(){};
};