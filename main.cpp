#include "Cells.hpp"
#include "Scene.hpp"

#define startHeight 900
#define startWidth 900

Scene scene;

int main(int argc, char* argv[]) {

    InitWindow(startWidth, startHeight, "Fluid Simulation");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    scene = Scene(vec2(startWidth, startHeight));

    while(!WindowShouldClose()){


        float dt = GetFrameTime();
        scene.CameraUpdate();

        BeginDrawing();
            ClearBackground(GRAY);
            BeginMode2D(scene.camera);

                scene.Draw();

            EndMode2D();
            DrawText(TextFormat("%.4f ms", GetFrameTime() * 1000.0f), 10, startHeight - 30.0f, 20, GREEN);
        EndDrawing();
    }

    CloseWindow();
}