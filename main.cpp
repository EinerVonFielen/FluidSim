#include "Scene.hpp"

#define startHeight 900
#define startWidth 900

Scene scene = Scene(vec2(startWidth, startHeight));

int main(int argc, char* argv[]) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(startWidth, startHeight, "Fluid Simulation");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    scene.CreateCells(ivec2(100, 100), 10);

    while(!WindowShouldClose()){

        if (IsWindowResized){
            vec2 newSize = vec2(GetRenderWidth(), GetRenderHeight());
            scene.SetSize(newSize);
        }

        float dt = GetFrameTime();

        scene.CameraUpdate();
        scene.Update(dt);

        BeginDrawing();
            ClearBackground(BLUEGRAY);
            BeginMode2D(scene.camera);

                scene.Draw();

            EndMode2D();
            DrawText(TextFormat("%.4f ms", GetFrameTime() * 1000.0f), 10, scene.windowSize.y - 30.0f, 20, GREEN);
        EndDrawing();
    }

    CloseWindow();
}