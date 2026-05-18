#include "Cells.hpp"
#include <cstdint>
#include <iostream>


Cells::Cells(ivec2 Size, float Cellsize){
    size = Size;
    cellsize = Cellsize;
    brushsize = 5 * cellsize;
    cellcount = size.x * size.y;
    cells = new Cell[cellcount];
    smoke = new float[cellcount];
    smoketemp = new float[cellcount];
    velocitiesX = new float[cellcount + size.y];
    velocitiesY = new float[cellcount + size.x];
    newvelocitiesX = new float[cellcount + size.y];
    newvelocitiesY = new float[cellcount + size.x];
    for (int y = 0; y < size.y; y++){
        for (int x = 0; x < size.x; x++){
            cells[y * size.x + x] = Cell(0.0f);
            if (x == 0 || x == size.x - 1 || y == 0 || y == size.y - 1) cells[y * size.x + x].notsolid = false;
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
           
            if (x > 0 && !cells[y * size.x + x - 1].notsolid){
                velocitiesX[y * (size.x + 1) + x] = 0;
                velocitiesX[y * (size.x + 1) + x - 1] = 0;
            }
        }
    }
    for (int y = 0; y < size.y + 1; y++){
        for (int x = 0; x < size.x; x++){
            //velocitiesY[y * size.x + x] = GetRandomValue(-1000, 1000) / 100.0f;
            velocitiesY[y * size.x + x] = 0;
            if (y > 0 && !cells[(y - 1) * size.x + x].notsolid){
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
    constexpr float maxvalue = 10000000.0f;
    f = glm::abs(glm::clamp(f, -maxvalue, maxvalue)) / maxvalue;
    float f_inv = 1.0f - f;
    return {static_cast<unsigned char>(GRAY1.r * f_inv + secondary.r * f),
            static_cast<unsigned char>(GRAY1.g * f_inv + secondary.g * f),
            static_cast<unsigned char>(GRAY1.b * f_inv + secondary.b * f),
            255};
}


Color DivergenceToColor(float f){
    Color secondary;
    if (f < 0){
        secondary = BLUE;
    } else {
        secondary = RED;
    }
    //std::cout << f << std::endl;
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


Color SmokeToColor(float f){

    f = glm::clamp(f, 0.0f, 10.0f) / 10.0f;
    float f_inv = 1 - f;
    return {static_cast<unsigned char>(GRAY1.r * f_inv + WHITE.r * f),
        static_cast<unsigned char>(GRAY1.g * f_inv + WHITE.g * f),
        static_cast<unsigned char>(GRAY1.b * f_inv + WHITE.b * f),
        255};
}


void Cells::Draw(){
    const int outline = cellsize / 20.0f;
    DrawRectangle(-outline, -outline, size.x * cellsize + outline * 2, size.y * cellsize + outline * 2, GRAY2);

    for (int y = 0; y < size.y; y++){
        for (int x = 0; x < size.x; x++){
            int i = y * size.x + x;
            Cell current_cell = cells[i];
            Color cellcolor;
            switch(selectedoverlay){
                case 6:
                    cellcolor = PressureToColor(current_cell.pressure);
                    break;
                case 7:
                    cellcolor = VelocityToColor(GetVelocity(ivec2(x, y)));
                    break;
                case 8:
                    cellcolor = DivergenceToColor(GetDivergence(ivec2(x, y)));
                    break;
                case 9:
                    cellcolor = SmokeToColor(smoke[i]);
                    break;
                default:
                    cellcolor = GRAY1;
                
            }
            cellimagedata[i] = cellcolor;
        }
    }

    UpdateTexture(celltexture, cellimage.data);
    DrawTextureEx(celltexture, {0, 0}, 0, cellsize, WHITE);


    if (shoulddrawarrow){
        for (int y = 0; y < size.y; y++){
            for (int x = 0; x < size.x; x++){
                if (!cells[y * size.x + x].notsolid) continue;
                vec2 pos = vec2(x, y) * cellsize + vec2(cellsize / 2.0f);
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
    if (dt <= 0) return;
    UpdateVelocities(dt);
    for (int i = 0; i < 80; i++){
        UpdatePressureWrapper(dt);
    }
    ApplyPressure(dt);

    AdvectSmoke(dt);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
        MouseVelocityChange(mousePos);
        MouseSolid(mousePos);
        SmokeMouse(mousePos);
    }

    ChangeBrush();
    if (IsKeyPressed(KEY_SPACE)) shoulddrawarrow = !shoulddrawarrow;
    drawbrushtimer -= dt;
    lastMousePos = mousePos;
}


float Cells::GetDivergence(ivec2 pos){
    int current_index = pos.y * size.x + pos.x;
    Cell& current_cell = cells[current_index];
    if (!current_cell.notsolid){
        return 0;
    }
    float velocityXLeft = velocitiesX[pos.y * (size.x + 1) + pos.x];
    float velocityXRight = velocitiesX[pos.y * (size.x + 1) + pos.x + 1];
    float velocityYUp = velocitiesY[pos.y * size.x + pos.x];
    float velocityYDown = velocitiesY[(pos.y + 1) * size.x + pos.x];
    return (velocityXRight - velocityXLeft + velocityYDown - velocityYUp) / cellsize;
}


vec2 Cells::GetVelocity(ivec2 pos){
    return vec2(velocitiesX[pos.y * (size.x + 1) + pos.x] + velocitiesX[pos.y * (size.x + 1) + pos.x + 1], velocitiesY[pos.y * size.x + pos.x] + velocitiesY[(pos.y + 1) * size.x + pos.x]) * 0.5f;
}


void Cells::UpdatePressure(float dt, int x, int y){
    
    int current_index = y * size.x + x;
    Cell& current_cell = cells[current_index];

    if (!current_cell.notsolid){
        current_cell.pressure = 0;
        return;
    }

    // Update pressure based on neighboring cells
    float pressure_sum = 0.0f;
    float velocity_difference_x = 0.0f;
    float velocity_difference_y = 0.0f;
    int neighbor_count = 0;

    // Check neighbors (up, down, left, right)
    //Left
    if (x > 0 && cells[current_index - 1].notsolid) {
        pressure_sum += cells[current_index - 1].pressure;
        neighbor_count++;
        velocity_difference_x -= velocitiesX[current_index + y];
    }
    //Right
    if (x < size.x - 1 && cells[current_index + 1].notsolid) {
        pressure_sum += cells[current_index + 1].pressure;
        neighbor_count++;
        velocity_difference_x += velocitiesX[current_index + 1 + y];
    }
    //Up
    if (y > 0 && cells[current_index - size.x].notsolid) {
        pressure_sum += cells[current_index - size.x].pressure;
        neighbor_count++;
        velocity_difference_y -= velocitiesY[current_index];
    }
    //Down
    if (y < size.y - 1 && cells[current_index + size.x].notsolid) {
        pressure_sum += cells[current_index + size.x].pressure;
        neighbor_count++;
        velocity_difference_y += velocitiesY[current_index + size.x];
    }
    if (neighbor_count == 0){
        current_cell.pressure = 0;
        return;
    }
    current_cell.pressure = pressure_sum / neighbor_count - (cellsize * (velocity_difference_x + velocity_difference_y)) / (neighbor_count * dt);
}


void Cells::UpdatePressureEdge(float dt, bool odd){
    
    bool isxeven = (size.x % 2) == 0;
    bool isyeven = (size.y % 2) == 0;

    for (int x = odd; x < size.x; x += 2){
        UpdatePressure(dt, x, 0);
    }
    for (int x = isyeven ? !odd : odd; x < size.x; x += 2){
        UpdatePressure(dt, x, size.y - 1);
    }
    for (int y = 1 + !odd; y < size.y - 1; y += 2){
        UpdatePressure(dt, 0, y);
    }
    for (int y = isxeven ? 1 + odd : 1 + !odd; y < size.y - 1; y += 2){
        UpdatePressure(dt, size.x - 1, y);
    }
}


void Cells::UpdatePressureMid(float dt, int start, int end, bool even){
    
    for (int y = start; y < end; y++){
        for (int x = (y + even) % 2 + 1; x < size.x - 1; x += 2){
            int current_index = y * size.x + x;
            Cell& current_cell = cells[current_index];

            if (!current_cell.notsolid){
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
            int valid;
            valid = cells[current_index - 1].notsolid;
            pressure_sum += cells[current_index - 1].pressure;
            neighbor_count += valid;
            velocity_difference_x -= velocitiesX[current_index + y] * valid;
            
            //Right
            valid = cells[current_index + 1].notsolid;
            pressure_sum += cells[current_index + 1].pressure;
            neighbor_count += valid;
            velocity_difference_x += velocitiesX[current_index + 1 + y] * valid;
            
            //Up
            valid = cells[current_index - size.x].notsolid;
            pressure_sum += cells[current_index - size.x].pressure;
            neighbor_count += valid;
            velocity_difference_y -= velocitiesY[current_index] * valid;
            
            //Down
            valid = cells[current_index + size.x].notsolid;
            pressure_sum += cells[current_index + size.x].pressure;
            neighbor_count += valid;
            velocity_difference_y += velocitiesY[current_index + size.x] * valid;
            
            if (neighbor_count == 0){
                current_cell.pressure = 0;
                continue;
            }
            current_cell.pressure = pressure_sum / neighbor_count - (cellsize * (velocity_difference_x + velocity_difference_y)) / (neighbor_count * dt);
        }
    }
}



void Cells::UpdatePressureWrapper(float dt){    
    
    const int numThreads = 6;
    int sizePerThread = static_cast<int>(size.y / numThreads);

    std::vector<std::thread> threads;

    for (int t = 0; t < numThreads; t++) {
        int start = t * sizePerThread;
        int end = (t == numThreads - 1) ? size.y : start + sizePerThread;
        threads.emplace_back(&Cells::UpdatePressureMid, this, dt, start, end, true);
    }
    threads.emplace_back(&Cells::UpdatePressureEdge, this, dt, false);

    for (auto& thread : threads) {
        thread.join();
    }
    threads.clear();

    for (int t = 0; t < numThreads; t++) {
        int start = t * sizePerThread;
        int end = (t == numThreads - 1) ? size.y : start + sizePerThread;
        threads.emplace_back(&Cells::UpdatePressureMid, this, dt, start, end, false);
    }
    threads.emplace_back(&Cells::UpdatePressureEdge, this, dt, true);

    for (auto& thread : threads) {
        thread.join();
    }
}

void Cells::ApplyPressure(float deltaTime){
    for (int y = 0; y < size.y ; y++){
        for (int x = 1; x < size.x; x++){
            float pressure_difference_x = cells[y * size.x + x].pressure - cells[y * size.x + x - 1].pressure;
            velocitiesX[y * (size.x + 1) + x] -= pressure_difference_x * deltaTime / cellsize;
            velocitiesX[y * (size.x + 1) + x] *= (cells[y * size.x + x].notsolid && cells[y * size.x + x - 1].notsolid);
        }
    }
    for (int y = 1; y < size.y; y++){
        for (int x = 0; x < size.x; x++){
            float pressure_difference_y = cells[y * size.x + x].pressure - cells[(y - 1) * size.x + x].pressure;
            velocitiesY[y * size.x + x] -= pressure_difference_y * deltaTime / cellsize;
            velocitiesY[y * size.x + x] *= (cells[y * size.x + x].notsolid && cells[(y - 1) * size.x + x].notsolid);
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


float Cells::SampleSmoke(vec2 ppos)
{
    float gx = ppos.x / cellsize - 0.5f;
    float gy = ppos.y / cellsize - 0.5f;

    int x0 = (int)gx;
    int y0 = (int)gy;

    float fx = gx - x0;
    float fy = gy - y0;

    int x1 = x0 + 1;
    int y1 = y0 + 1;

    x0 = std::clamp(x0, 0, size.x - 1);
    x1 = std::clamp(x1, 0, size.x - 1);
    y0 = std::clamp(y0, 0, size.y - 1);
    y1 = std::clamp(y1, 0, size.y - 1);

    float s00 = smoke[y0 * size.x + x0];
    float s10 = smoke[y0 * size.x + x1];
    float s01 = smoke[y1 * size.x + x0];
    float s11 = smoke[y1 * size.x + x1];

    float sx0 = s00 * (1 - fx) + s10 * fx;
    float sx1 = s01 * (1 - fx) + s11 * fx;

    return sx0 * (1 - fy) + sx1 * fy;
}


void Cells::AdvectSmoke(float dt){
    for (int y = 1; y < size.y - 1; y++){
        for (int x = 1; x < size.x - 1; x++){
            int i = y * size.x + x;

            vec2 vel = GetVelocity(ivec2(x, y));

            vec2 ppos = vec2(x, y) * cellsize + vec2(cellsize / 2.0f) - vel * dt;
            ppos.x = std::clamp(ppos.x, 0.0f, size.x * cellsize);
            ppos.y = std::clamp(ppos.y, 0.0f, size.y * cellsize);

            smoketemp[i] = SampleSmoke(ppos);
        }
    }
    float* temp = smoke;
    smoke = smoketemp;
    smoketemp = temp;
}


void Cells::MouseVelocityChange(Vector2 mousePos){
    if (selectedbrush != 3) return;
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


void Cells::MouseSolid(Vector2 mousePos){
    if (!(selectedbrush == 1 || selectedbrush == 2)) return;
    if (mousePos.x < 0 || mousePos.y < 0 || mousePos.x > cellsize * size.x || mousePos.y > cellsize * size.y) return;
    int cellbrushsize = static_cast<int>(brushsize / cellsize);
    ivec2 mousepos = ivec2(static_cast<int>(mousePos.x / cellsize), static_cast<int>(mousePos.y / cellsize));

    int xmin = glm::max(0, mousepos.x - cellbrushsize);
    int xmax = glm::min(size.x, mousepos.x + cellbrushsize + 1);
    int ymin = glm::max(0, mousepos.y - cellbrushsize);
    int ymax = glm::min(size.y, mousepos.y + cellbrushsize + 1);

    for (int x = xmin; x < xmax; x++){
        int heightlimit = static_cast<int>(glm::sqrt(cellbrushsize * cellbrushsize - (x - mousepos.x) * (x - mousepos.x)));
        int newymin = glm::max(ymin, mousepos.y - heightlimit);
        int newymax = glm::min(ymax, mousepos.y + heightlimit + 1);
        for (int y = newymin; y < newymax; y++){
            cells[y * size.x + x].notsolid = static_cast<bool>(selectedbrush - 1);
            cells[y * size.x + x].pressure = 0;
            smoke[y * size.x + x] = 0.0f;
        }
    }
}


void Cells::SmokeMouse(Vector2 mousePos){
    if (selectedbrush != 4) return;
    if (mousePos.x < 0 || mousePos.y < 0 || mousePos.x > cellsize * size.x || mousePos.y > cellsize * size.y) return;
    int cellbrushsize = static_cast<int>(brushsize / cellsize);
    ivec2 mousepos = ivec2(static_cast<int>(mousePos.x / cellsize), static_cast<int>(mousePos.y / cellsize));

    int xmin = glm::max(0, mousepos.x - cellbrushsize);
    int xmax = glm::min(size.x, mousepos.x + cellbrushsize + 1);
    int ymin = glm::max(0, mousepos.y - cellbrushsize);
    int ymax = glm::min(size.y, mousepos.y + cellbrushsize + 1);

    for (int x = xmin; x < xmax; x++){
        int heightlimit = static_cast<int>(glm::sqrt(cellbrushsize * cellbrushsize - (x - mousepos.x) * (x - mousepos.x)));
        int newymin = glm::max(ymin, mousepos.y - heightlimit);
        int newymax = glm::min(ymax, mousepos.y + heightlimit + 1);
        for (int y = newymin; y < newymax; y++){
            smoke[y * size.x + x] = 10.0f;
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


void Cells::ChangeBrush(){
    for (int i = 1; i <= 5; i++){
        if (IsKeyPressed(KEY_ONE + i - 1)){
            selectedbrush = i;
            break;
        }
    }
    for (int i = 6; i <= 9; i++){
        if (IsKeyPressed(KEY_ONE + i - 1)){
            selectedoverlay = i;
            break;
        }
    }
}


Cells::~Cells(){
    delete[] cells;
    delete[] velocitiesX;
    delete[] velocitiesY;
};