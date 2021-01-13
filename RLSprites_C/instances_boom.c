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

SpriteInstance* ActiveSprites[512] = { 0 };
int LastSprite = -1;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "RLSprites [c version] example - Boom");
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    InitAudioDevice();

    Sound boom = LoadSound("resources/boom.wav");
    Sprite sprite = { 0 };
    AddSpriteImageFromFile(&sprite, "resources/explosion.png", 5, 5);

    AddSpriteAnimationFrames(&sprite, "boom", DIRECTION_DEFAULT, 0, 24);
    SetSpriteAnimationLoop(&sprite, "boom", false);
    SetSpriteAnimationSpeed(&sprite, "boom", 30);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update

        if (IsMouseButtonPressed(0))
        {
            SpriteInstance* instance = CreateSpriteInstance(&sprite, WHITE);
            SetSpriteInstanceAimation(instance, "boom");
            instance->Direction = DIRECTION_DEFAULT;
            instance->OriginX = OriginCenter;
            instance->OriginY = OriginCenter;
            instance->Postion = GetMousePosition();

            LastSprite++;
            if (LastSprite >= 512)
                LastSprite = 0;
            ActiveSprites[LastSprite] = instance;

            PlaySoundMulti(boom);
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int i = 0; i < 512; i++)
        {
            if (ActiveSprites[i] != NULL)
            {
                if (ActiveSprites[i]->LastFrameTime > 100)
                {
                    FreeSpriteInstance(ActiveSprites[i]);
                    ActiveSprites[i] = NULL;
                }
                else
                    UpdateRenderSpriteInstance(ActiveSprites[i]);
            }   
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    FreeSprite(&sprite);
    UnloadSound(boom);
    CloseAudioDevice();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}