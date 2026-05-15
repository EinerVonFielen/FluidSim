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
            cells[y * size.x + x] = Cell(vec2(x, y) * cellsize, -2.0f, cellsize);
            if (x == 0 || x == size.x - 1 || y == 0 || y == size.y - 1) cells[y * size.x + x].solid = true;
        }
    }
    SetVelocities();
    cellimage = GenImageColor(size.x, size.y, WHITE);
    cellimagedata = (Color*) cellimage.data;
    celltexture = LoadTextureFromImage(cellimage);
}


void Cells::SetVelocities(){
    for (int y = 0; y < size.y; y++){
        for (int x = 0; x < size.x + 1; x++){
            velocitiesX[y * (size.x + 1) + x] = GetRandomValue(-1000, 1000) / 1000.0f;
            if (x > 0 && cells[y * size.x + x - 1].solid){
                velocitiesX[y * (size.x + 1) + x] = 0;
                velocitiesX[y * (size.x + 1) + x - 1] = 0;
            }
        }
    }
    for (int y = 0; y < size.y + 1; y++){
        for (int x = 0; x < size.x; x++){
            velocitiesY[y * size.x + x] = GetRandomValue(-1000, 1000) / 1000.0f;
            if (y > 0 && cells[(y - 1) * size.x + x].solid){
                velocitiesY[y * size.x + x] = 0;
                velocitiesY[(y - 1) * size.x + x - 1] = 0;
            }
        }
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
        Color cellcolor = PressureToColor(current_cell.pressure);
        cellimagedata[i] = cellcolor;
        //DrawRectangle(current_cell.position.x + outline, current_cell.position.y + outline, cellsize - outline * 2.0f, cellsize - outline * 2.0f, cellcolor);
    }

    UpdateTexture(celltexture, cellimage.data);
    DrawTextureEx(celltexture, {0, 0}, 0, cellsize, WHITE);


    if (!shoulddrawarrow) return;
    for (int y = 0; y < size.y; y++){
        for (int x = 0; x < size.x; x++){
            if (cells[y * size.x + x].solid) continue;
            vec2 pos = cells[y * size.x + x].position + vec2(cellsize / 2.0f);
            vec2 arrow = vec2(velocitiesX[y * (size.x + 1) + x] + velocitiesX[y * (size.x + 1) + x + 1], velocitiesY[y * size.x + x] + velocitiesY[(y + 1) * size.x + x]) * 0.5f;
            arrow *= cellsize / 2.0f;
            DrawLineEx({pos.x, pos.y}, {pos.x + arrow.x, pos.y + arrow.y}, cellsize / 20.0f, WHITE);
            DrawCircle(pos.x, pos.y, cellsize / 20.0f, WHITE);
        }
    }
}


void Cells::Update(float dt, Vector2 mousePos){
    for (int i = 0; i < 50; i++){
        UpdatePressure(dt);
    }
    UpdateVelocities(dt);

    MouseVelocityChange(mousePos);
    if (IsKeyPressed(KEY_SPACE)) shoulddrawarrow = !shoulddrawarrow;
}


void Cells::UpdatePressure(float dt){
    if (dt <= 0) return;
    for (int y = 0; y < size.y; y++){
        for (int x = 0; x < size.x; x++){
            int current_index = y * size.x + x;
            Cell& current_cell = cells[current_index];

            if (current_cell.solid){
                current_cell.pressure = 0;
                continue;
            }

            // Update pressure based on neighboring cells
            float pressure_sum = 0.0f;
            float velocity_difference_x = 0.0f;
            float velocity_difference_y = 0.0f;
            int neighbor_count = 0;

            // Check neighbors (up, down, left, right)
            //Left
            if (x > 0 && !cells[current_index - 1].solid) {
                pressure_sum += cells[current_index - 1].pressure;
                neighbor_count++;
                velocity_difference_x -= velocitiesX[current_index + y];
            }
            //Right
            if (x < size.x - 1 && !cells[current_index + 1].solid) {
                pressure_sum += cells[current_index + 1].pressure;
                neighbor_count++;
                velocity_difference_x += velocitiesX[current_index + 1 + y];
            }
            //Up
            if (y > 0 && !cells[current_index - size.x].solid) {
                pressure_sum += cells[current_index - size.x].pressure;
                neighbor_count++;
                velocity_difference_y -= velocitiesY[current_index];
            }
            //Down
            if (y < size.y - 1 && ! cells[current_index + size.x].solid) {
                pressure_sum += cells[current_index + size.x].pressure;
                neighbor_count++;
                velocity_difference_y += velocitiesY[current_index + size.x];
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
            velocitiesX[y * (size.x + 1) + x] *= !(cells[y * size.x + x].solid || cells[y * size.x + x - 1].solid);
        }
    }
    for (int y = 1; y < size.y; y++){
        for (int x = 0; x < size.x; x++){
            float pressure_difference_y = cells[y * size.x + x].pressure - cells[(y - 1) * size.x + x].pressure;
            velocitiesY[y * size.x + x] -= pressure_difference_y * deltaTime / cellsize;
            velocitiesY[y * size.x + x] *= !(cells[y * size.x + x].solid || cells[(y - 1) * size.x + x].solid);
        }
    }
}


void Cells::MouseVelocityChange(Vector2 mousePos){
    Vector2 mouseDiff = {mousePos.x - lastMousePos.x, mousePos.y - lastMousePos.y};
    lastMousePos = mousePos;

    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) return;
    constexpr float brushsize = 100.0f;
    constexpr float brushstrength = 0.01f;

    for (int y = 0; y < size.y; y++){
        for (int x = 0; x < size.x + 1; x++){
            Vector2 pos = {x * cellsize, y * cellsize + cellsize / 2.0f};
            Vector2 dist = {pos.x - mousePos.x, pos.y - mousePos.y};
            float r = glm::sqrt(dist.x * dist.x + dist.y * dist.y);
            if (r <= brushsize){
                velocitiesX[y * (size.x + 1) + x] += mouseDiff.x * brushstrength * ((brushsize - r) / brushsize);
            }
        }
    }
    for (int y = 0; y < size.y + 1; y++){
        for (int x = 0; x < size.x; x++){
            Vector2 pos = {x * cellsize + cellsize / 2.0f, y * cellsize};
            Vector2 dist = {pos.x - mousePos.x, pos.y - mousePos.y};
            float r = glm::sqrt(dist.x * dist.x + dist.y * dist.y);
            if (r <= brushsize){
                velocitiesY[y * size.x + x] += mouseDiff.y * brushstrength / r;
            }
        }
    }
}


Cells::~Cells(){
    delete[] cells;
    delete[] velocitiesX;
    delete[] velocitiesY;
};