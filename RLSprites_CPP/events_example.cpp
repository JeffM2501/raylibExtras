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

std::vector<Sound> Footsteps;
int FootstepIndex = 0;

void walkFootstepCallback(SpriteInstance* instance, int frame)
{
    PlaySoundMulti(Footsteps[FootstepIndex]);
    FootstepIndex++;
    if (FootstepIndex >= Footsteps.size())
        FootstepIndex = 0;
}

void dieEndCallback(SpriteInstance* instance, int frame)
{
    instance->SetAimation("die_reverse");
    walkFootstepCallback(instance, 0);
}

void dieReverseEndCallback(SpriteInstance* instance, int frame)
{
    instance->SetAimation("die");
}

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    InitAudioDevice();

    Footsteps.push_back(LoadSound("resources/sounds/stepdirt_1.wav"));
    Footsteps.push_back(LoadSound("resources/sounds/stepdirt_2.wav"));
    Footsteps.push_back(LoadSound("resources/sounds/stepdirt_3.wav"));
    Footsteps.push_back(LoadSound("resources/sounds/stepdirt_4.wav"));
    Footsteps.push_back(LoadSound("resources/sounds/stepdirt_5.wav"));
    Footsteps.push_back(LoadSound("resources/sounds/stepdirt_6.wav"));
    Footsteps.push_back(LoadSound("resources/sounds/stepdirt_7.wav"));
    Footsteps.push_back(LoadSound("resources/sounds/stepdirt_8.wav"));

    Sprite baseSprite;
    baseSprite.AddImage("resources/png/walkcycle/BODY_skeleton.png", 9, 4);
    baseSprite.AddImage("resources/png/hurt/BODY_skeleton.png", 6, 1);

    Sprite hatSprite;
    hatSprite.AddImage("resources/png/walkcycle/HEAD_chain_armor_helmet.png", 9, 4);
    hatSprite.AddImage("resources/png/hurt/HEAD_chain_armor_helmet.png", 6, 1);

    Sprite pantsSprite;
    pantsSprite.AddImage("resources/png/walkcycle/LEGS_pants_greenish.png", 9, 4);
    pantsSprite.AddImage("resources/png/hurt/LEGS_pants_greenish.png", 6, 1);

    SpriteAnimation idleAnim;
    idleAnim.Name = "idle";
    idleAnim.DirectionFrames[0].push_back(0);
    idleAnim.DirectionFrames[1].push_back(9);
    idleAnim.DirectionFrames[2].push_back(18);
    idleAnim.DirectionFrames[3].push_back(27);
    idleAnim.Loop = true;

    baseSprite.AddAnimation(idleAnim);

    baseSprite.AddAnimation("walk", DIRECTION_UP, 1, 8);
    baseSprite.AddAnimation("walk", DIRECTION_LEFT, 10, 17);
    baseSprite.AddAnimation("walk", DIRECTION_DOWN, 19, 26);
    baseSprite.AddAnimation("walk", DIRECTION_RIGHT, 28, 35);

    baseSprite.AddAnimationFrameCallback("walk", walkFootstepCallback, 2);
    baseSprite.AddAnimationFrameCallback("walk", walkFootstepCallback, 6);

    baseSprite.SetAnimationLoop("walk", true);
    baseSprite.SetAnimationSpeed("walk", 12);

    baseSprite.AddAnimation("die", DIRECTION_DEFAULT, 36, 41);
    baseSprite.AddAnimation("die_reverse", DIRECTION_DEFAULT, 41, 36);

    baseSprite.AddAnimationFrameCallback("die", dieEndCallback, -1);
    baseSprite.AddAnimationFrameCallback("die_reverse", dieReverseEndCallback, -1);


    SpriteInstance skelly1(baseSprite);
    skelly1.Layers.push_back(SpriteInstance::Layer{ &hatSprite, WHITE });
    skelly1.Layers.push_back(SpriteInstance::Layer{ &pantsSprite, GREEN });
    skelly1.SetAimation("idle");

    skelly1.OriginX = OriginLocations::Center;
    skelly1.OriginY = OriginLocations::Center;

    skelly1.Postion = Vector2{ 100,100 };
    skelly1.Scale = 1.0f;

    SpriteInstance skelly2(baseSprite);
    skelly2.Layers.push_back(SpriteInstance::Layer{ &hatSprite, RED });
    skelly2.Layers.push_back(SpriteInstance::Layer{ &pantsSprite, RED });
    skelly2.SetAimation("die");

    skelly2.OriginX = OriginLocations::Center;
    skelly2.OriginY = OriginLocations::Center;

    skelly2.Postion = Vector2{ 300,100 };
    skelly2.Scale = 1.0f;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        float speed = GetFrameTime() * 35;

        bool moving = true;
        if (IsKeyDown(KEY_LEFT))
        {
            skelly1.Direction = 1;
            moving = true;
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            skelly1.Direction = 3;
            moving = true;
        }
        else if (IsKeyDown(KEY_UP))
        {
            skelly1.Direction = 0;
            moving = true;
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            skelly1.Direction = 2;
            moving = true;
        }
        else
            moving = false;

        if (IsKeyDown(KEY_LEFT_SHIFT) && moving)
            skelly1.Speed = 2;
        else
            skelly1.Speed = 1;

        if (IsKeyDown(KEY_LEFT))
        {
            skelly1.Postion.x -= speed * skelly1.Speed;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            skelly1.Postion.x += speed * skelly1.Speed;
        }
        if (IsKeyDown(KEY_UP))
        {
            skelly1.Postion.y -= speed * skelly1.Speed;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            skelly1.Postion.y += speed * skelly1.Speed;
        }
		
        skelly1.SetAimation(moving ? "walk" : "idle");

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(WHITE);

        skelly2.UpdateRender();
        skelly1.UpdateRender();

        DrawText(TextFormat("pos %3.0f %3.0f", skelly1.Postion.x, skelly1.Postion.y), 10, 10, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    for (auto& sound : Footsteps)
        UnloadSound(sound);

    CloseAudioDevice();     // Close audio device
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}