// RLTilesTest.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include "raylib.h"
#include "RLTiles.h"

int main()
{
    InitWindow(800, 450, "RLTiles test");

    RLTileMap tileMap;

    RLReadTileMap("Resources/Maps/orthogonal-outside.tmx",tileMap);
   
    for (std::map<int, RLTileSheet>::iterator itr = tileMap.Sheets.begin(); itr != tileMap.Sheets.end(); itr++)
    {
        itr->second.SheetSource = "resources/" + itr->second.SheetSource;
    }

    RLTileRenderer renderer(tileMap);
    renderer.Setup();

    Camera2D cam = { 0 };
    cam.offset = Vector2{ 0,0 };
    cam.target = Vector2{ 0,0 };
    cam.rotation = 0;
    cam.zoom = 1;

    while (!WindowShouldClose())
    {
        float speed = 500 * GetFrameTime();
        if (IsKeyDown(KEY_S))
            cam.target.y += speed;

        if (IsKeyDown(KEY_W))
            cam.target.y -= speed;

        if (IsKeyDown(KEY_D))
            cam.target.x += speed;

        if (IsKeyDown(KEY_A))
            cam.target.x -= speed;


        if (IsKeyPressed(KEY_E))
            cam.zoom *= 1.15f;
        if (IsKeyPressed(KEY_C))
            cam.zoom *= 0.95f;

        cam.offset.x = GetScreenWidth() * 0.5f;
        cam.offset.y = GetScreenHeight() * 0.5f;

        BeginDrawing();
        ClearBackground(BLUE);

        BeginMode2D(cam);
        renderer.Draw(cam);
        EndMode2D();

        DrawFPS(0, 0);

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context
}
