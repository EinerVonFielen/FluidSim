#include <raylib.h>
#include <glm/vec2.hpp>
#include "Cell.hpp"
#include <iostream>
glm::vec2 position = { 0.0f, 0.0f };
static int startHeight;
static int startWidth;
static int cellcount;


void Draw(){
    std::cout << "2 Drawing cell at (" << position.x << ", " << position.y << ")" << std::endl; // Debug output
    DrawRectangle(position.x/startHeight, position.y/startWidth, startHeight/cellcount, startWidth/cellcount, RED);
}
void updateDensity(){
    
}



