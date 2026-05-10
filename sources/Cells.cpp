#include "Cells.hpp"


Cells::Cells(vec2 Size){

    size = Size;
    cellcount = static_cast<int>(size.x * size.y);
    cells = new Cell[cellcount];


    for (int y = 0; y < size.y; y++){
        for (int x = 0; x < size.x; x++){
            
        }
    }
}


Cells::~Cells(){
    delete cells;
}