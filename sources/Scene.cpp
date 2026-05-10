#include "Scene.hpp"

void Scene::CameraUpdate(){
    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) || IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mouseDelta = GetMouseDelta();
        camera.target.x -= mouseDelta.x / camera.zoom;
        camera.target.y -= mouseDelta.y / camera.zoom;
    }

    Vector2 mouseWorldPosBefore = GetScreenToWorld2D(GetMousePosition(), camera);

    float wheel = GetMouseWheelMove();
    if (wheel != 0){
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
    DrawRectangle(0, 0, 300, 200, LIGHTGRAY);
}