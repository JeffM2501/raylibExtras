/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   RLSprite * Simple Sprite Managment System for Raylib
*
*   LICENSE: MIT
*
*   Copyright (c) 2020 Jeffery Myers
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/

#include "raylib.h"
#include "RLSprites.h"

using namespace RLSprites;

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    InitAudioDevice();
    Sound step = LoadSound("resources/sounds/stepdirt_1.wav");

    Sprite baseSprite = Sprite::Load("resources/scarfy.sprite");

    SpriteInstance sprite(baseSprite);
    sprite.SetAimation("idle");
    sprite.Direction = DIRECTION_LEFT;

    sprite.OriginX = OriginLocations::Center;
    sprite.OriginY = OriginLocations::Center;

    sprite.Position = Vector2{ 100,100 };
    sprite.Scale = 1.0f;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        float speed = GetFrameTime() * 400;

        bool moving = true;
        if (IsKeyDown(KEY_LEFT))
        {
            sprite.Direction = DIRECTION_LEFT;
            moving = true;
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            sprite.Direction = DIRECTION_RIGHT;
            moving = true;
        }
        else
            moving = false;

        if (IsKeyDown(KEY_LEFT_SHIFT) && moving)
            sprite.Speed = 2;
        else
            sprite.Speed = 1;

        if (IsKeyDown(KEY_LEFT))
        {
            sprite.Position.x -= speed * sprite.Speed;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            sprite.Position.x += speed * sprite.Speed;
        }

        sprite.SetAimation(moving ? "walk" : "idle");

        sprite.Update();
        if (sprite.TriggerFrameName == "step")
            PlaySoundMulti(step);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(WHITE);

        sprite.Render();

        DrawText("(c) Scarfy sprite by Eiden Marsal", screenWidth - 200, screenHeight - 20, 10, GRAY);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    UnloadSound(step);
    CloseAudioDevice();
    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}