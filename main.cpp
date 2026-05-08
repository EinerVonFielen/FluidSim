#include <raylib.h>
#include <iostream>
#include "glm/glm.hpp"

#define startHeight 540
#define startWidth 960


int main(int argc, char* argv[]) {

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(startWidth, startHeight, "Evolution");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    glm::vec2 test = glm::vec2(0);

    while(!WindowShouldClose()){


        float dt = GetFrameTime();

        BeginDrawing();
            ClearBackground(GRAY);

        EndDrawing();
    }

    CloseWindow();
}