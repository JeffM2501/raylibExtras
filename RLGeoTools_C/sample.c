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
#include "rlgl.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "RLGeoTools.h"

void BuildMesh(Mesh* mesh, void* userData)
{
    Vector3 minOrigin = { 0, 0, 0 };
    Vector3 maxOrigin = { 1.0f, 1.0f, .0f };

    float minV = 0;

    float vertices[] =
    {
        minOrigin.x,maxOrigin.y,maxOrigin.z,
        minOrigin.x,minOrigin.y,maxOrigin.z,
        maxOrigin.x,maxOrigin.y,maxOrigin.z,
        maxOrigin.x,minOrigin.y,maxOrigin.z,
    };

    float texcoords[] =
    {
        1,minV,
        1,1,
        0,minV,
        0,1,
    };

    float normals[] =
    {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
    };

    AllocateMeshData(mesh, 4, 2, 0, true, false);
    memcpy(mesh->vertices, vertices, 4 * 3 * sizeof(float));
    memcpy(mesh->texcoords, texcoords, 4 * 2 * sizeof(float));
    memcpy(mesh->normals, normals, 4 * 3 * sizeof(float));

    mesh->indices[0] = 2;
    mesh->indices[1] = 1;
    mesh->indices[2] = 0;

    mesh->indices[3] = 2;
    mesh->indices[4] = 3;
    mesh->indices[5] = 1;

    mesh->vertexCount = 4;
    mesh->triangleCount = 2;
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib extras, GeoTools");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
  
    Texture tx = LoadTextureFromImage(GenImageChecked(256, 256, 128, 128, LIGHTGRAY, BLUE));
    SetTextureFilter(tx, FILTER_ANISOTROPIC_8X);

    Mesh myMesh = GenMeshCustom(BuildMesh, NULL);

    Material mat = LoadMaterialDefault();
    mat.maps[MAP_DIFFUSE].texture = tx;
    mat.maps[MAP_DIFFUSE].color = WHITE;

    Vector3 ppos = { 0, 0, 0};

    Camera camera;
    camera.fovy = 45;
    camera.position = (Vector3){ 0, 1, -5 };
    camera.target = (Vector3){ 0,0,0 };
    camera.type = CAMERA_PERSPECTIVE;
    camera.up = (Vector3){ 0,1,0 };

    SetCameraMode(camera, CAMERA_FREE);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
            ClearBackground(BLACK);

            UpdateCamera(&camera);

            BeginMode3D(camera);
            {
                DrawGrid(10, 1.0f);
                rlDrawMesh(myMesh, mat, MatrixIdentity());
            }
            EndMode3D();
            DrawFPS(10, 10);
        }
        EndDrawing();
    }
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}