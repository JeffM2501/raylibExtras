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

    InitAudioDevice();

    Sound step = LoadSound("resources/sounds/stepdirt_1.wav");

    Sprite sprite = { 0 };

    bool loadFromFile = false; // set to true to load from sprite file
    if (loadFromFile)
    {
        LoadSpriteInfo(&sprite, "resources/scarfy.sprite");
    }
    else
    {
       
        AddSpriteImageFromFile(&sprite, "resources/scarfy.png", 6, 1);      // define base image and frames
        AddSpriteFlipFrames(&sprite, 0, 5, true, false);                    // flip frames 0 to 5 to make left running sequence

        AddSpriteAnimationFrames(&sprite, "walk", DIRECTION_RIGHT, 0, 5);   // right running is 0 to 5
        AddSpriteAnimationFrames(&sprite, "walk", DIRECTION_LEFT, 6, 11);   // left running is flipped frames 6 to 11

        AddSpriteAnimationFrameCallback(&sprite, "walk", "step", 1, NULL);  // add named event for each frame where the foot hits the ground
        AddSpriteAnimationFrameCallback(&sprite, "walk", "step", 4, NULL);
        AddSpriteAnimationFrameCallback(&sprite, "walk", "step", 7, NULL);
        AddSpriteAnimationFrameCallback(&sprite, "walk", "step", 10, NULL);

        SetSpriteAnimationLoop(&sprite, "walk", true);                      // walk loops
        SetSpriteAnimationSpeed(&sprite, "walk", 12);                       // walk runs at 12fps by default

        AddSpriteAnimationFrames(&sprite, "idle", DIRECTION_RIGHT, 2, 2);   // idle for each direction
        AddSpriteAnimationFrames(&sprite, "idle", DIRECTION_LEFT, 8, 8);
        SetSpriteAnimationLoop(&sprite, "idle", true);
        SetSpriteAnimationSpeed(&sprite, "idle", 1);

        // SaveSpriteInfo(&sprite, "resources/scarfy.sprite");              // saves the sprite and animations to a sprite file
    }

    SpriteInstance* instance = CreateSpriteInstance(&sprite, WHITE);        // create an instance of our sprite to draw
    SetSpriteInstanceAimation(instance, "idle");
    instance->Direction = DIRECTION_LEFT;
    instance->OriginX = OriginCenter;
    instance->OriginY = OriginCenter;
    instance->Position = (Vector2){ 100,100 };

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        float speed = GetFrameTime() * 400;     // time based speed

        // movement and input.
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

        // sprint
        if (IsKeyDown(KEY_LEFT_SHIFT) && moving)
            instance->Speed = 2;
        else
            instance->Speed = 1;

        if (IsKeyDown(KEY_LEFT))
        {
            instance->Position.x -= speed * instance->Speed;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            instance->Position.x += speed * instance->Speed;
        }

        // set animation based on state
        SetSpriteInstanceAimation(instance, moving ? "walk" : "idle");

        // update the sprite frame
        UpdateSpriteInstance(instance);

        // check and see if we triggered a named frame on this update
        if (SpriteInstanceFrameTriggered(instance, "step"))
            PlaySoundMulti(step);
       
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        RenderSpriteInstance(instance);

        DrawText("(c) Scarfy sprite by Eiden Marsal", screenWidth - 200, screenHeight - 20, 10, GRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    UnloadSound(step);
    FreeSpriteInstance(instance);
    instance = NULL;
    FreeSprite(&sprite);

    CloseAudioDevice();

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}