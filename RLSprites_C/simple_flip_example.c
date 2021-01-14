/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "RLSprites.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "RLSprites [c version] example - simple flip");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    Sprite sprite = { 0 };
    AddSpriteImageFromFile(&sprite, "resources/scarfy.png", 6, 1);
    AddSpriteFlipFrames(&sprite, 0, 5, true, false);

    AddSpriteAnimationFrames(&sprite, "walk", DIRECTION_RIGHT, 0, 5);
    AddSpriteAnimationFrames(&sprite, "walk", DIRECTION_LEFT, 6, 11);
    SetSpriteAnimationLoop(&sprite, "walk", true);
    SetSpriteAnimationSpeed(&sprite, "walk", 12);

    AddSpriteAnimationFrames(&sprite, "idle", DIRECTION_RIGHT, 2, 2);
    AddSpriteAnimationFrames(&sprite, "idle", DIRECTION_LEFT, 8, 8);
    SetSpriteAnimationLoop(&sprite, "idle", true);
    SetSpriteAnimationSpeed(&sprite, "idle", 1);

    SpriteInstance* instance = CreateSpriteInstance(&sprite, WHITE);
    SetSpriteInstanceAimation(instance, "idle");
    instance->Direction = DIRECTION_LEFT;
    instance->OriginX = OriginCenter;
    instance->OriginY = OriginCenter;
    instance->Postion = (Vector2){ 100,100 };

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        float speed = GetFrameTime() * 400;

        bool moving = true;
        if (IsKeyDown(KEY_LEFT))
        {
            instance->Direction = DIRECTION_LEFT;
            moving = true;
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            instance->Direction = DIRECTION_RIGHT;
            moving = true;
        }
        else
            moving = false;

        if (IsKeyDown(KEY_LEFT_SHIFT) && moving)
            instance->Speed = 2;
        else
            instance->Speed = 1;

        if (IsKeyDown(KEY_LEFT))
        {
            instance->Postion.x -= speed * instance->Speed;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            instance->Postion.x += speed * instance->Speed;
        }
        SetSpriteInstanceAimation(instance, moving ? "walk" : "idle");

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        UpdateRenderSpriteInstance(instance);
        DrawText("(c) Scarfy sprite by Eiden Marsal", screenWidth - 200, screenHeight - 20, 10, GRAY);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    FreeSpriteInstance(instance);
    instance = NULL;
    FreeSprite(&sprite);

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}