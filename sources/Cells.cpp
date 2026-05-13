#include "Cells.hpp"


Cells::Cells(ivec2 Size, float Cellsize){
    size = Size;
    cellsize = Cellsize;
    cellcount = size.x * size.y;
    cells = new Cell[cellcount];
    velocitiesX = new float[cellcount + size.y];
    velocitiesY = new float[cellcount + size.x];
    for (int y = 0; y < size.y; y++){
        for (int x = 0; x < size.x; x++){
            if ((x == 0 || x == size.x - 1) && (y == 0 || y == size.y - 1)){
                cells[y * size.x + x] = Cell(vec2(x, y) * cellsize, 0.0f, cellsize, true);
            } else {
                cells[y * size.x + x] = Cell(vec2(x, y) * cellsize, 0.0f, cellsize, false);
            }
        }
    }
    velocitiesX = new float[cellcount + size.y];
    for (int i = 0; i < cellcount + size.y; i++){
        velocitiesX[i] = GetRandomValue(-1000, 1000) / 1000.0f;
    }
    velocitiesY = new float[cellcount + size.x];
    for (int i = 0; i < cellcount + size.x; i++){
        velocitiesY[i] = GetRandomValue(-1000, 1000) / 1000.0f;
    }
}


Color PressureToColor(float f){
    Color secondary;
    if (f < 0){
        secondary = BLUE;
    } else {
        secondary = RED;
    }
    constexpr float maxvalue = 10.0f;
    f = glm::abs(glm::clamp(f, -maxvalue, maxvalue)) / maxvalue;
    float f_inv = 1.0f - f;
    return {static_cast<unsigned char>(GRAY1.r * f_inv + secondary.r * f),
            static_cast<unsigned char>(GRAY1.g * f_inv + secondary.g * f),
            static_cast<unsigned char>(GRAY1.b * f_inv + secondary.b * f),
            255};
}


void Cells::Draw(){
    const int outline = cellsize / 20.0f;
    DrawRectangle(-outline, -outline, size.x * cellsize + outline * 2, size.y * cellsize + outline * 2, GRAY2);

    for (int i = 0; i < cellcount; i++){   
        Cell current_cell = cells[i];
        DrawRectangle(current_cell.position.x + outline, current_cell.position.y + outline, cellsize - outline * 2.0f, cellsize - outline * 2.0f, PressureToColor(current_cell.pressure));
    }

    for (int y = 0; y < size.y; y++){
        for (int x = 0; x < size.x + 1; x++){
            DrawLineEx({x * cellsize, y * cellsize + cellsize / 2.0f}, {x * cellsize + velocitiesX[y * (size.x + 1) + x] * 50.0f, y * cellsize + cellsize / 2.0f}, 3, WHITE);
        }
    }
    for (int y = 0; y < size.y + 1; y++){
        for (int x = 0; x < size.x; x++){
            DrawLineEx({x * cellsize + cellsize / 2.0f, y * cellsize}, {x * cellsize + cellsize / 2.0f, y * cellsize + velocitiesY[y * size.x + x] * 50.0f}, 3, WHITE);
        }
    }
}


void Cells::Update(float dt){
    for (int i = 0; i < 30; i++){
        UpdatePressure(dt);
    }
    UpdateVelocities(dt * 0.2);
}


void Cells::UpdatePressure(float dt){
    if (dt <= 0) return;
    for (int y = 0; y < size.y; y++){
        for (int x = 0; x < size.x; x++){
            int current_index = y * size.x + x;
            Cell& current_cell = cells[current_index];

            // Update pressure based on neighboring cells

            float pressure_sum = 0.0f;
            float velocity_difference_x = 0.0f;
            float velocity_difference_y = 0.0f;
            int neighbor_count = 0;

            // Check neighbors (up, down, left, right)
            //Left
            if (x > 0) {
                if (cells[current_index - 1].solid) continue;
                pressure_sum += cells[current_index - 1].pressure;
                neighbor_count++;
                velocity_difference_x -= velocitiesX[current_index + y];

            }
            //Right
            if (x < size.x - 1) {
                if (cells[current_index + 1].solid) continue;
                pressure_sum += cells[current_index + 1].pressure;
                neighbor_count++;
                velocity_difference_x += velocitiesX[current_index + 1 + y];
            }
            //Up
            if (y > 0) {
                if (cells[current_index - size.x].solid) continue;
                pressure_sum += cells[current_index - size.x].pressure;
                neighbor_count++;
                velocity_difference_y -= velocitiesY[current_index];
            }
            //Down
            if (y < size.y - 1) {
                if (cells[current_index + size.x].solid) continue;
                pressure_sum += cells[current_index + size.x].pressure;
                neighbor_count++;
                velocity_difference_y += velocitiesY[current_index + size.y];
            }
            current_cell.pressure = pressure_sum / neighbor_count - (cellsize * (velocity_difference_x + velocity_difference_y)) / (neighbor_count * dt);
        }
    }
}

void Cells::UpdateVelocities(float deltaTime){
    for (int y = 0; y < size.y; y++){
        for (int x = 1; x < size.x; x++){
            float pressure_difference_x = cells[y * size.x + x].pressure - cells[y * size.x + x - 1].pressure;
            velocitiesX[y * (size.x + 1) + x] -= pressure_difference_x * deltaTime / cellsize;
        }
    }
    for (int y = 1; y < size.y; y++){
        for (int x = 0; x < size.x; x++){
            float pressure_difference_y = cells[y * size.x + x].pressure - cells[(y - 1) * size.x + x].pressure;
            velocitiesY[y * size.x + x] -= pressure_difference_y * deltaTime / cellsize;
        }
    }
}


Cells::~Cells(){
    delete[] cells;
    delete[] velocitiesX;
    delete[] velocitiesY;
};