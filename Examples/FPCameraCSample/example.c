/*******************************************************************************************
*
*   raylib [core] example - Third Person Orbit Camera Example
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
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include "FPCamera.h"
#include "rlgl.h"

#include "Frustum.h"
#include "RLGeoTools.h"

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 1900;
	int screenHeight = 900;

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	InitWindow(screenWidth, screenHeight, "raylib [camera] example - First person camera");
	//SetTargetFPS(120);

	//--------------------------------------------------------------------------------------

	Image img = GenImageChecked(256, 256, 32, 32, DARKGRAY, WHITE);
	Texture tx = LoadTextureFromImage(img);
	UnloadImage(img);
	SetTextureFilter(tx, TEXTURE_FILTER_ANISOTROPIC_16X);
	SetTextureWrap(tx, TEXTURE_WRAP_CLAMP);

	Image skyImg = LoadImage("resources/Daylight Box UV.png");
	TextureCubemap skyboxTexture = LoadTextureCubemap(skyImg, CUBEMAP_LAYOUT_AUTO_DETECT);
	UnloadImage(skyImg);

	Model skybox = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));

	// Load skybox shader and set options in required locations
	SetModelMaterialShader(&skybox,0,LoadShaderSet("resources/shaders","skybox"));
	SetModelMaterialShaderValue(&skybox, 0, "environmentMap", (int[1]) { MATERIAL_MAP_CUBEMAP }, SHADER_UNIFORM_INT);
	SetModelMaterialShaderValue(&skybox, 0, "noGamma", (int[1]) { 1 }, SHADER_UNIFORM_INT);
	SetModelMaterialTexture(&skybox, 0, MATERIAL_MAP_CUBEMAP, skyboxTexture);

	double shakeStart = -1;
	float shakeMag = 0;
	float shakeFreq = 0;

	float shakeDecay = 0.5f;

	// setup initial camera data
	FPCamera cam;
	InitFPCamera(&cam, 45, (Vector3) { 1, 0, 0 });
	cam.MoveSpeed.z = 10;
	cam.MoveSpeed.x = 5;

	cam.FarPlane = 5000;

	Frustum viewFrustum = { 0 };

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		if (IsKeyPressed(KEY_SPACE))
		{
			if (shakeStart < 0)
				shakeStart = GetTime();

			shakeFreq = 50;
			shakeMag = 0.125f;
		}

		float shakeOffset = 0;
		if (shakeStart > 0)
		{
			shakeMag -= GetFrameTime() * shakeDecay;
			shakeFreq -= GetFrameTime() * shakeDecay;

			if (shakeMag < 0 || shakeFreq < 0)
				shakeStart = -1;
			else
			{
				float delta = (float)(GetTime() - shakeStart);
				shakeOffset = sinf(delta * shakeFreq) * shakeMag;
			}
		}

		UpdateFPCamera(&cam);

		BeginDrawing();
		ClearBackground(WHITE);

		Camera3D viewCam = cam.ViewCamera;
		viewCam.position = Vector3Add(viewCam.position, Vector3Scale(cam.Right, shakeOffset));

		BeginMode3D(viewCam);
		ExtractFrustum(&viewFrustum);

		rlDisableBackfaceCulling();     // Disable backface culling to render inside the cube
		rlDisableDepthMask();			// Disable depth writes
		rlDisableDepthTest();			// Disable depth test for speed

		DrawModel(skybox, (Vector3) { 0, 0, 0 }, 1.0f, WHITE);

		rlEnableBackfaceCulling();     // enable things we turned off
		rlEnableDepthMask();
		rlEnableDepthTest();

		// grid of cube trees on a plane to make a "world"
		DrawPlane((Vector3) { 0, 0, 0 }, (Vector2) { 50, 50 }, BEIGE); // simple world plane
		float spacing = 4;
		int count = 5;

		int total = 0;
		int vis = 0;

		for (float x = -count * spacing; x <= count * spacing; x += spacing)
		{
			for (float z = -count * spacing; z <= count * spacing; z += spacing)
			{
				Vector3 pos = { x, 0.5f, z };

				Vector3 min = { x - 0.5f,0,z - 0.5f };
				Vector3 max = { x + 0.5f,1,z + 0.5f };
				total++;
			//	if (AABBoxInFrustum(&viewFrustum, min, max))
				{
					DrawCubeTexture(tx, (Vector3) { x, 1.5f, z }, 1, 1, 1, GREEN);
					DrawCubeTexture(tx, (Vector3) { x, 0.5f, z }, 0.25f, 1, 0.25f, BROWN);
					vis++;
				}
			}
		}

		EndMode3D();

	//	DrawText(TextFormat("%d visible of %d total", vis, total), 200, 0, 20, GREEN);
		// instructions
		DrawFPS(0, 0);
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}