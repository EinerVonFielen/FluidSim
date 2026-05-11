#include "Cells.hpp"


Cells::Cells(vec2 Size, float Cellsize){
    size = Size;
    cellsize = Cellsize;
    cellcount = static_cast<int>(size.x * size.y);
    cells = new Cell[cellcount];

    for (int y = 0; y < size.y; y++){
        for (int x = 0; x < size.x; x++){
            cells[y * static_cast<int>(size.x) + x] = Cell(vec2(x, y) * cellsize, 0.0f, cellsize);
        }
    }
    //velocitiesX = new float[cellcount + size.y];
    //velocitiesY = new float[cellcount + size.x];
}


void Cells::Draw(){
    const int outline = cellsize / 20.0f;
    DrawRectangle(-outline, -outline, size.x * cellsize + outline * 2, size.y * cellsize + outline * 2, GRAY2);

    for (int i = 0; i < size.x; i++){
        for (int j = 0; j < size.y; j++){
            //std::cout << " 1 Drawing cell at (" << i << ", " << j << ")" << std::endl; // Debug output
            Cell current_cell = cells[i * static_cast<int>(size.y) + j];
            DrawRectangle(current_cell.position.x + outline, current_cell.position.y + outline, cellsize - outline * 2.0f, cellsize - outline * 2.0f, GRAY1);
        }
    }
}


void Cells::Update(){
    
}


Cells::~Cells(){
    delete[] cells;
    //delete[] velocitiesX;
    //delete[] velocitiesY;
};