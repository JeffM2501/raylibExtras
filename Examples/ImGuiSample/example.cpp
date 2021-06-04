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

#include "imgui.h"
#include "rlImGui.h"

bool Quit = false;

bool ImGuiDemoOpen = false;
bool ImageEditorWindowOpen = true;

RenderTexture ImageScratchPad;
Texture ImageTexture;
Camera2D ScratchCamera = { 0 };

Vector2 LastMousePos = { 0 };
Vector2 LastTarget = { 0 };
bool Dragging = false;

enum class ToolMode
{
	None,
	Move,
	Zoom
};

ToolMode CurrentToolMode = ToolMode::None;

void DoMainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
				Quit = true;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::MenuItem("ImGui Demo", nullptr, &ImGuiDemoOpen);
			ImGui::MenuItem("Image Editor", nullptr, &ImageEditorWindowOpen);

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void ShowImageEditor()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

	if (ImGui::Begin("Image Viewer", &ImageEditorWindowOpen,ImGuiWindowFlags_NoScrollbar))
	{
		if (ImGui::BeginChild("Toolbar",ImVec2(ImGui::GetContentRegionAvailWidth(),25)))
		{
			ImGui::SetCursorPosX(2);
			ImGui::SetCursorPosY(3);

            if (ImGui::Button("None"))
            {
				CurrentToolMode = ToolMode::None;
            }
            ImGui::SameLine();

			if (ImGui::Button("Move"))
			{
                CurrentToolMode = ToolMode::Move;
			}
			ImGui::SameLine();

			if (ImGui::Button("Zoom"))
			{
                CurrentToolMode = ToolMode::Zoom;
			}

			ImGui::SameLine();
			switch (CurrentToolMode)
			{
			case ToolMode::None:
				ImGui::TextUnformatted("No Tool");
				break;
			case ToolMode::Move:
				ImGui::TextUnformatted("Move Tool");
				break;
			case ToolMode::Zoom:
				ImGui::TextUnformatted("Zoom Tool");
				break;
			default:
				break;
			}

			ImGui::SameLine();
			ImGui::TextUnformatted(TextFormat("camera target X%f Y%f", ScratchCamera.target.x, ScratchCamera.target.y));
			ImGui::EndChild();
		}


		ImVec2 size = ImGui::GetContentRegionAvail();

		// center the scratchpad in the view
		Rectangle viewRect = { 0 };
		viewRect.x = ImageScratchPad.texture.width / 2 - size.x / 2;
		viewRect.y = ImageScratchPad.texture.height / 2 - size.y / 2;
		viewRect.width = size.x;
		viewRect.height = size.y;

		// draw the scratchpad
		RLImGuiImageRect(&ImageScratchPad.texture, (int)size.x, (int)size.y, viewRect);

		
		ImGui::SetCursorPos(ImVec2(0, 0));
		// some tools

		ImGui::End();
	}
	ImGui::PopStyleVar();
}

void UpdateRenderTexture()
{
	BeginTextureMode(ImageScratchPad);
	ClearBackground(BLUE);
	BeginMode2D(ScratchCamera);
	DrawTexture(ImageTexture, ImageTexture.width/-2, ImageTexture.height/-2,WHITE);
	EndMode2D();
	EndTextureMode();
}

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 1900;
	int screenHeight = 900;

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	InitWindow(screenWidth, screenHeight, "raylib [ImGui] example - ImGui Demo");
	//SetTargetFPS(120);
	SetupRLImGui(true);
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

	ImageScratchPad = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	Image testImage = GenImageCellular(800, 800, 25);
	ImageTexture = LoadTextureFromImage(testImage);
	UnloadImage(testImage);

	ScratchCamera.zoom = 1;
	ScratchCamera.target.x = 0;
	ScratchCamera.target.y = 0;
	ScratchCamera.rotation = 0;
	ScratchCamera.offset.x = GetScreenWidth() / 2.0f;
	ScratchCamera.offset.y = GetScreenHeight() / 2.0f;

	UpdateRenderTexture();

	LastMousePos = GetMousePosition();

	// Main game loop
	while (!WindowShouldClose() && !Quit)    // Detect window close button or ESC key
	{
		if (IsWindowResized())
		{
			UnloadRenderTexture(ImageScratchPad);
			ImageScratchPad = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

            ScratchCamera.offset.x = GetScreenWidth() / 2.0f;
            ScratchCamera.offset.y = GetScreenHeight() / 2.0f;
		}

		if (CurrentToolMode == ToolMode::Move)
		{
			if (IsMouseButtonDown(0))
			{
				if (!Dragging)
				{
                    LastMousePos = GetMousePosition();
                    LastTarget = ScratchCamera.target;
				}
				Dragging = true;
                Vector2 mousePos = GetMousePosition();
                Vector2 mouseDelta = Vector2Subtract(LastMousePos, mousePos);

				mouseDelta.x /= ScratchCamera.zoom;
				mouseDelta.y /= ScratchCamera.zoom;
                ScratchCamera.target = Vector2Add(LastTarget, mouseDelta);

				UpdateRenderTexture();
			}
			else
			{
				Dragging = false;
			}
		}
		else if (CurrentToolMode == ToolMode::Zoom)
		{
			if (IsMouseButtonPressed(0))
			{
				Vector2 mousePos = GetMousePosition();
				Vector2 localCameraPos = GetScreenToWorld2D(mousePos, ScratchCamera);
				ScratchCamera.target = localCameraPos;

				if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
					ScratchCamera.zoom /= 1.25f;
				else
					ScratchCamera.zoom *= 1.25f;

                UpdateRenderTexture();
			}
		}

		BeginDrawing();
		ClearBackground(DARKGRAY);

		BeginRLImGui();
		DoMainMenu();

		if (ImGuiDemoOpen)
			ImGui::ShowDemoWindow(&ImGuiDemoOpen);

		if (ImageEditorWindowOpen)
			ShowImageEditor();

		EndRLImGui();

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
	ShutdownRLImGui();
	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}