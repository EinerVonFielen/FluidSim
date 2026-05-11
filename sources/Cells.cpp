#include "Cells.hpp"
#include <iostream>
#include <raylib.h>

vec2 size;
Cell* cells;
int cellcount;
int startHeight;
int startWidth;
float* velocitiesX;
float* velocitiesY;
Cells::Cells(vec2 size, int startHeight, int startWidth){
    this->size = size;
    this->startHeight = startHeight;
    this->startWidth = startWidth;
    cellcount = static_cast<int>(size.x * size.y);
    cells = new Cell[cellcount];

    for (int i = 0; i < size.y; i++){
        for (int j = 0; j < size.x; j++){
            cells[i * static_cast<int>(size.x) + j] = Cell(j, i, 0.0f, startHeight, startWidth, cellcount);
        }
    }
    //velocitiesX = new float[cellcount + size.y];
    //velocitiesY = new float[cellcount + size.x];
}
void Cells::Draw(){
    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            std::cout << " 1 Drawing cell at (" << i << ", " << j << ")" << std::endl; // Debug output
            cells[i * width + j].Draw();
        }
    }
}




Cells::~Cells(){
        delete[] cells;
        delete[] velocitiesX;
        delete[] velocitiesY;
    };