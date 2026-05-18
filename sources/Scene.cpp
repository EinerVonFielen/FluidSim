#include "Scene.hpp"
#include "Cells.hpp"

void Scene::CreateCells(ivec2 size, float cellsize){
    if (cells) delete cells;
    cells = new Cells(size, cellsize);
    camera.target = {size.x * cellsize * 0.5f, size.y * cellsize * 0.5f};
}


Scene::~Scene(){
    if (cells) delete cells;
}


void Scene::CameraUpdate(){
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        Vector2 mouseDelta = GetMouseDelta();
        camera.target.x -= mouseDelta.x / camera.zoom;
        camera.target.y -= mouseDelta.y / camera.zoom;
    }

    Vector2 mouseWorldPosBefore = GetScreenToWorld2D(GetMousePosition(), camera);

    float wheel = GetMouseWheelMove();
    if (wheel != 0 && !IsKeyDown(KEY_LEFT_SHIFT)){
        camera.zoom = expf(logf(camera.zoom) + wheel * 0.1f);
        Vector2 mouseWorldPosAfter = GetScreenToWorld2D(GetMousePosition(), camera);

        camera.target.x += mouseWorldPosBefore.x - mouseWorldPosAfter.x;
        camera.target.y += mouseWorldPosBefore.y - mouseWorldPosAfter.y;
    }

    if (IsKeyPressed(KEY_R)){
        camera.zoom = 0.005f;
        camera.rotation = 0.0f;
    }
}


void Scene::Draw(){
    if (cells) cells->Draw();
}


void Scene::Update(float dt){
    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
    if (cells) cells->Update(dt, mousePos);
}