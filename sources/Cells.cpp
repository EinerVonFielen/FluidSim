#include "Cells.hpp"


Cells::Cells(vec2 Size, float Cellsize){
    size = Size;
    cellsize = Cellsize;
    cellcount = static_cast<int>(size.x * size.y);
    cells = new Cell[cellcount];
    velocitiesX = new float[cellcount + static_cast<int>(size.y)];
    velocitiesY = new float[cellcount + static_cast<int>(size.x)];
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


void Cells::UpdatePressure(float deltaTime){
    for (int i = 0; i < size.x; i++){
        for (int j = 0; j < size.y; j++){
            Cell& current_cell = cells[i * static_cast<int>(size.y) + j];
            // Update pressure based on neighboring cells
            float pressure_sum = 0.0f;
            float velocity_difference_x = 0.0f;
            float velocity_difference_y = 0.0f;
            int neighbor_count = 0;
            // Check neighbors (up, down, left, right)
            if (i > 0) {
                pressure_sum += cells[(i - 1) * static_cast<int>(size.y) + j].pressure;
                neighbor_count++;
                velocity_difference_x -= velocitiesX[(i - 1) * static_cast<int>(size.y) + j];

            }
            if (i < size.x - 1) {
                pressure_sum += cells[(i + 1) * static_cast<int>(size.y) + j].pressure;
                neighbor_count++;
                velocity_difference_x += velocitiesX[(i + 1) * static_cast<int>(size.y) + j];
            }
            if (j > 0) {
                pressure_sum += cells[i * static_cast<int>(size.y) + (j - 1)].pressure;
                neighbor_count++;
                velocity_difference_y -= velocitiesY[i * static_cast<int>(size.y) + (j - 1)];
            }
            if (j < size.y - 1) {
                pressure_sum += cells[i * static_cast<int>(size.y) + (j + 1)].pressure;
                neighbor_count++;
                velocity_difference_y += velocitiesY[i * static_cast<int>(size.y) + (j + 1)];
            }
            
           
            current_cell.pressure = pressure_sum / nearbyint(neighbor_count) - (cellsize * (velocity_difference_x + velocity_difference_y)) / (nearbyint(neighbor_count) * deltaTime);

        }
    }
}

void Cells::UpdateVelocities(float deltaTime){
    for (int i = 0; i < size.x; i++){
        for (int j = 0; j < size.y +1; j++){
            float pressure_difference_x = 0.0f;
            if (i > 0) {
                pressure_difference_x += cells[(i - 1) * static_cast<int>(size.y) + j].pressure;
            }
            if (i < size.x - 1) {
                pressure_difference_x -= cells[(i + 1) * static_cast<int>(size.y) + j].pressure;
            }
            velocitiesX[i * static_cast<int>(size.y) + j] += pressure_difference_x * deltaTime / cellsize;
        }
    }
    for (int i = 0; i < size.x +1; i++){
        for (int j = 0; j < size.y; j++){
            float pressure_difference_y = 0.0f;
            if (j > 0) {
                pressure_difference_y += cells[i * static_cast<int>(size.y) + (j - 1)].pressure;
            }
            if (j < size.y - 1) {
                pressure_difference_y -= cells[i * static_cast<int>(size.y) + (j + 1)].pressure;
            }
            velocitiesY[i * static_cast<int>(size.y) + j] += pressure_difference_y * deltaTime / cellsize;
        }
    }
}


Cells::~Cells(){
    delete[] cells;
    //delete[] velocitiesX;
    //delete[] velocitiesY;
};