#include "Scene.hpp"
#include <chrono>

#define startHeight 900
#define startWidth 900
#define framerate 60
#define secondsperprint 2

Scene scene = Scene(vec2(startWidth, startHeight));

int main(int argc, char* argv[]) {

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(startWidth, startHeight, "Fluid Simulation");
    SetTargetFPS(framerate);
    SetExitKey(KEY_NULL);

    scene.CreateCells(ivec2(100, 60), 40);

    unsigned long int framecounter = 1, drawtime = 0, updatetime = 0;

    while(!WindowShouldClose()){

        if (IsWindowResized){
            vec2 newSize = vec2(GetRenderWidth(), GetRenderHeight());
            scene.SetSize(newSize);
        }

        float dt = GetFrameTime();

        scene.CameraUpdate();

        auto startupdate = std::chrono::system_clock::now();
        scene.Update(dt);
        updatetime += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - startupdate).count();

        BeginDrawing();
            ClearBackground(BLUEGRAY);
            BeginMode2D(scene.camera);

                auto startdraw = std::chrono::system_clock::now();
                scene.Draw();
                drawtime += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - startdraw).count();

            EndMode2D();
            DrawText(TextFormat("%.4f ms", GetFrameTime() * 1000.0f), 10, scene.windowSize.y - 30.0f, 20, GREEN);
        EndDrawing();

        if (framecounter == secondsperprint * framerate){
            std::cout << "--------------------" << std::endl;
            std::cout << "Time taken to update: " << (updatetime / (framerate * secondsperprint)) / 1000.0f << " ms" << std::endl;
            std::cout << "Time taken to draw:   " << (drawtime / (framerate * secondsperprint)) / 1000.0f << " ms" << std::endl;
            framecounter = 1;
            drawtime = 0;
            updatetime = 0;
        } else framecounter++;
    }

    CloseWindow();
}