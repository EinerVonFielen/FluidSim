#include <raylib.h>
#include <iostream>
#include "glm/glm.hpp"
#include "includes/cells.hpp"

#define startHeight 900
#define startWidth 900


int main(int argc, char* argv[]) {

    InitWindow(startWidth, startHeight, "Evolution");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);


    while(!WindowShouldClose()){


        float dt = GetFrameTime();

        BeginDrawing();
            ClearBackground(GRAY);
            DrawText(TextFormat("%.4f ms", GetFrameTime() * 1000.0f), 10, startHeight - 30.0f, 20, GREEN);
        EndDrawing();
    }

    CloseWindow();
}