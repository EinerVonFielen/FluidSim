#include "Cells.hpp"
#include <iostream>


Cells::Cells(ivec2 Size, float Cellsize){
    size = Size;
    cellsize = Cellsize;
    brushsize = 5 * cellsize;
    cellcount = size.x * size.y;
    cells = new Cell[cellcount];
    velocitiesX = new float[cellcount + size.y];
    velocitiesY = new float[cellcount + size.x];
    newvelocitiesX = new float[cellcount + size.y];
    newvelocitiesY = new float[cellcount + size.x];
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
            //velocitiesX[y * (size.x + 1) + x] = GetRandomValue(-1000, 1000) / 100.0f;
            velocitiesX[y * (size.x + 1) + x] = 0;
           
            if (x > 0 && cells[y * size.x + x - 1].solid){
                velocitiesX[y * (size.x + 1) + x] = 0;
                velocitiesX[y * (size.x + 1) + x - 1] = 0;
            }
        }
    }
    for (int y = 0; y < size.y + 1; y++){
        for (int x = 0; x < size.x; x++){
            //velocitiesY[y * size.x + x] = GetRandomValue(-1000, 1000) / 100.0f;
            velocitiesY[y * size.x + x] = 0;
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
    //std::cout << f << std::endl;
    f = f / 500000.0f;
    constexpr float maxvalue = 10.0f;
    f = glm::abs(glm::clamp(f, -maxvalue, maxvalue)) / maxvalue;
    float f_inv = 1.0f - f;
    return {static_cast<unsigned char>(GRAY1.r * f_inv + secondary.r * f),
            static_cast<unsigned char>(GRAY1.g * f_inv + secondary.g * f),
            static_cast<unsigned char>(GRAY1.b * f_inv + secondary.b * f),
            255};
}


Color VelocityToColor(vec2 vel){

    float f = glm::length(vel) / 15000.0f;
    f = glm::min(f, 1.0f);
    float f_inv = 1.0f - f;
    return {static_cast<unsigned char>(GRAY1.r * f_inv + RED.r * f),
            static_cast<unsigned char>(GRAY1.g * f_inv + RED.g * f),
            static_cast<unsigned char>(GRAY1.b * f_inv + RED.b * f),
            255};
}


void Cells::Draw(){
    const int outline = cellsize / 20.0f;
    DrawRectangle(-outline, -outline, size.x * cellsize + outline * 2, size.y * cellsize + outline * 2, GRAY2);

    for (int y = 0; y < size.y; y++){
        for (int x = 0; x < size.x; x++){
            int i = y * size.x + x;
            Cell current_cell = cells[i];
            //Color cellcolor = PressureToColor(current_cell.pressure);
            Color cellcolor = VelocityToColor(vec2(velocitiesX[y * (size.x + 1) + x] + velocitiesX[y * (size.x + 1) + x + 1], velocitiesY[y * size.x + x] + velocitiesY[(y + 1) * size.x + x]) * 0.5f);
            cellimagedata[i] = cellcolor;
            //DrawRectangle(current_cell.position.x + outline, current_cell.position.y + outline, cellsize - outline * 2.0f, cellsize - outline * 2.0f, cellcolor);
        }
    }

    UpdateTexture(celltexture, cellimage.data);
    DrawTextureEx(celltexture, {0, 0}, 0, cellsize, WHITE);


    if (shoulddrawarrow){
        for (int y = 0; y < size.y; y++){
            for (int x = 0; x < size.x; x++){
                if (cells[y * size.x + x].solid) continue;
                vec2 pos = cells[y * size.x + x].position + vec2(cellsize / 2.0f);
                vec2 arrow = vec2(velocitiesX[y * (size.x + 1) + x] + velocitiesX[y * (size.x + 1) + x + 1], velocitiesY[y * size.x + x] + velocitiesY[(y + 1) * size.x + x]) * 0.5f;
                arrow /= 30.0f;
                DrawLineEx({pos.x, pos.y}, {pos.x + arrow.x, pos.y + arrow.y}, cellsize / 20.0f, WHITE);
                DrawCircle(pos.x, pos.y, cellsize / 20.0f, WHITE);
            }
        }
    }

    DrawMouseCircle();
}


void Cells::Update(float dt, Vector2 mousePos){
    UpdateVelocities(dt);
    for (int i = 0; i < 80; i++){
        UpdatePressure(dt);
    }
    UpdateVelocitiesForDivergence(dt);
    UpdateDivergence();
    MouseVelocityChange(mousePos);

    if (IsKeyPressed(KEY_SPACE)) shoulddrawarrow = !shoulddrawarrow;
    drawbrushtimer -= dt;
    lastMousePos = mousePos;
}


void Cells::UpdateDivergence(){
    for (int y = 0; y < size.y; y++){
        for (int x = 0; x < size.x; x++){
            int current_index = y * size.x + x;
            Cell& current_cell = cells[current_index];
            if (current_cell.solid){
                current_cell.divergence = 0;
                continue;
            }
            float velocityXLeft = velocitiesX[y * (size.x + 1) + x];
            float velocityXRight = velocitiesX[y * (size.x + 1) + x + 1];
            float velocityYUp = velocitiesY[y * size.x + x];
            float velocityYDown = velocitiesY[(y + 1) * size.x + x];
            current_cell.divergence = (velocityXRight - velocityXLeft + velocityYDown - velocityYUp) / cellsize;
        }
    }
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

void Cells::UpdateVelocitiesForDivergence(float deltaTime){
    for (int y = 0; y < size.y ; y++){
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


vec2 Cells::GetVelocityAtPosition(vec2 pos)
{
    // In Grid-Koordinaten umrechnen
    float gx = pos.x / cellsize;
    float gy = pos.y / cellsize;

    //
    // =========================
    // Sample U (velocitiesX)
    // =========================
    // U liegt auf vertikalen Faces:
    // (x, y + 0.5)
    //

    float ux = gx;
    float uy = gy - 0.5f;

    int ux0 = (int)std::floor(ux);
    int uy0 = (int)std::floor(uy);

    float utx = ux - ux0;
    float uty = uy - uy0;

    int ux1 = ux0 + 1;
    int uy1 = uy0 + 1;

    ux0 = std::clamp(ux0, 0, size.x);
    ux1 = std::clamp(ux1, 0, size.x);

    uy0 = std::clamp(uy0, 0, size.y - 1);
    uy1 = std::clamp(uy1, 0, size.y - 1);

    float u00 = velocitiesX[uy0 * (size.x + 1) + ux0];
    float u10 = velocitiesX[uy0 * (size.x + 1) + ux1];
    float u01 = velocitiesX[uy1 * (size.x + 1) + ux0];
    float u11 = velocitiesX[uy1 * (size.x + 1) + ux1];

    float velocityX =
        (1.0f - utx) * (1.0f - uty) * u00 +
        utx * (1.0f - uty) * u10 +
        (1.0f - utx) * uty * u01 +
        utx * uty * u11;

    //
    // =========================
    // Sample V (velocitiesY)
    // =========================
    // V liegt auf horizontalen Faces:
    // (x + 0.5, y)
    //

    float vx = gx - 0.5f;
    float vy = gy;

    int vx0 = (int)std::floor(vx);
    int vy0 = (int)std::floor(vy);

    float vtx = vx - vx0;
    float vty = vy - vy0;

    int vx1 = vx0 + 1;
    int vy1 = vy0 + 1;

    vx0 = std::clamp(vx0, 0, size.x - 1);
    vx1 = std::clamp(vx1, 0, size.x - 1);

    vy0 = std::clamp(vy0, 0, size.y);
    vy1 = std::clamp(vy1, 0, size.y);

    float v00 = velocitiesY[vy0 * size.x + vx0];
    float v10 = velocitiesY[vy0 * size.x + vx1];
    float v01 = velocitiesY[vy1 * size.x + vx0];
    float v11 = velocitiesY[vy1 * size.x + vx1];

    float velocityY =
        (1.0f - vtx) * (1.0f - vty) * v00 +
        vtx * (1.0f - vty) * v10 +
        (1.0f - vtx) * vty * v01 +
        vtx * vty * v11;

    return { velocityX, velocityY };
}



void Cells::UpdateVelocities(float dt){
    for (int y = 0; y < size.y; y++){
        for (int x = 0; x < size.x + 1; x++){
            vec2 Pos = {x * cellsize, y * cellsize + cellsize / 2.0f};
            Pos -= GetVelocityAtPosition(Pos) * dt;
            newvelocitiesX[y * (size.x + 1) + x] = GetVelocityAtPosition(Pos).x;
        }
    }
    for (int y = 0; y < size.y + 1; y++){
        for (int x = 0; x < size.x; x++){
            vec2 Pos = {x * cellsize + cellsize / 2.0f, y * cellsize};
            Pos -= GetVelocityAtPosition(Pos) * dt;
            newvelocitiesY[y * size.x + x] = GetVelocityAtPosition(Pos).y;
        }
    }
    float* temp = velocitiesX;
    velocitiesX = newvelocitiesX;
    newvelocitiesX = temp;
    temp = velocitiesY;
    velocitiesY = newvelocitiesY;
    newvelocitiesY = temp;
}

void Cells::MouseVelocityChange(Vector2 mousePos){
    if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) return;
    Vector2 mouseDiff = {mousePos.x - lastMousePos.x, mousePos.y - lastMousePos.y};
    constexpr float brushstrength = 20.0f;

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
                velocitiesY[y * size.x + x] += mouseDiff.y * brushstrength * ((brushsize - r) / brushsize);
            }
        }
    }
}


void Cells::DrawMouseCircle(){ 
    if (IsKeyDown(KEY_LEFT_SHIFT)){
        brushsize *= GetMouseWheelMove() * 0.1f + 1.0f;
        drawbrushtimer = 0.3f;
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) drawbrushtimer = 0.5f;

    if (drawbrushtimer >= 0.0f){
        DrawCircle(lastMousePos.x, lastMousePos.y, brushsize, {255, 255, 255, static_cast<unsigned char>(150 * drawbrushtimer)});
    }
}


Cells::~Cells(){
    delete[] cells;
    delete[] velocitiesX;
    delete[] velocitiesY;
};