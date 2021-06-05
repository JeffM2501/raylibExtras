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

#include "FPCamera.h"

bool Quit = false;

bool ImGuiDemoOpen = false;

class DocumentWindow
{
public:
	bool Open = false;

	RenderTexture ViewTexture;

    virtual void Setup() = 0;
    virtual void Shutdown() = 0;
	virtual void Show() = 0;
	virtual void Update() = 0;

	bool Focused = false;
};


class ImageViewerWindow : public DocumentWindow
{
public:

	void Setup() override
	{
        Camera.zoom = 1;
        Camera.target.x = 0;
        Camera.target.y = 0;
        Camera.rotation = 0;
        Camera.offset.x = GetScreenWidth() / 2.0f;
        Camera.offset.y = GetScreenHeight() / 2.0f;

		ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        ImageTexture = LoadTexture("resources/parrots.png");

		UpdateRenderTexture();
	}

	void Show() override
	{
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

        if (ImGui::Begin("Image Viewer", &Open, ImGuiWindowFlags_NoScrollbar))
        {
            Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

            if (ImGui::BeginChild("Toolbar", ImVec2(ImGui::GetContentRegionAvailWidth(), 25)))
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
                ImGui::TextUnformatted(TextFormat("camera target X%f Y%f", Camera.target.x, Camera.target.y));
                ImGui::EndChild();
            }

            ImVec2 size = ImGui::GetContentRegionAvail();

            // center the scratchpad in the view
            Rectangle viewRect = { 0 };
            viewRect.x = ViewTexture.texture.width / 2 - size.x / 2;
            viewRect.y = ViewTexture.texture.height / 2 - size.y / 2;
            viewRect.width = size.x;
            viewRect.height = -size.y;

            // draw the scratchpad
            RLImGuiImageRect(&ViewTexture.texture, (int)size.x, (int)size.y, viewRect);

            ImGui::SetCursorPos(ImVec2(0, 0));
            // some tools

            ImGui::End();
        }
        ImGui::PopStyleVar();
	}

	void Update() override
	{
        if (!Open)
            return;

        if (IsWindowResized())
        {
            UnloadRenderTexture(ViewTexture);
			ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

            Camera.offset.x = GetScreenWidth() / 2.0f;
            Camera.offset.y = GetScreenHeight() / 2.0f;
        }

		if (Focused)
		{
            if (CurrentToolMode == ToolMode::Move)
            {
                if (IsMouseButtonDown(0))
                {
                    if (!Dragging)
                    {
                        LastMousePos = GetMousePosition();
                        LastTarget = Camera.target;
                    }
                    Dragging = true;
                    Vector2 mousePos = GetMousePosition();
                    Vector2 mouseDelta = Vector2Subtract(LastMousePos, mousePos);

                    mouseDelta.x /= Camera.zoom;
                    mouseDelta.y /= Camera.zoom;
                    Camera.target = Vector2Add(LastTarget, mouseDelta);

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
                    Vector2 localCameraPos = GetScreenToWorld2D(mousePos, Camera);
                    Camera.target = localCameraPos;

                    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
                        Camera.zoom /= 1.25f;
                    else
                        Camera.zoom *= 1.25f;

                    UpdateRenderTexture();
                }
            }
		}
		else
		{
			Dragging = false;
		}
	}

    Texture ImageTexture;
    Camera2D Camera = { 0 };

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

    void UpdateRenderTexture()
    {
        BeginTextureMode(ViewTexture);
        ClearBackground(BLUE);
        BeginMode2D(Camera);
        DrawTexture(ImageTexture, ImageTexture.width / -2, ImageTexture.height / -2, WHITE);
        EndMode2D();
        EndTextureMode();
    }

    void Shutdown() override
    {
        UnloadRenderTexture(ViewTexture);
        UnloadTexture(ImageTexture);
    }
};

class SceneViewWindow : public DocumentWindow
{
public:

    void Setup() override
    {
        ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

        Camera.HideCursor = false;
        Camera.Setup(45, Vector3{ 0,1,0 });

        Camera.MoveSpeed.z = 10;
        Camera.MoveSpeed.x = 10;

        Image img = GenImageChecked(256, 256, 32, 32, DARKGRAY, WHITE);
        GridTexture = LoadTextureFromImage(img);
        UnloadImage(img);
        SetTextureFilter(GridTexture, TEXTURE_FILTER_ANISOTROPIC_16X);
        SetTextureWrap(GridTexture, TEXTURE_WRAP_CLAMP);
    }

    void Shutdown() override
    {
        UnloadRenderTexture(ViewTexture);
        UnloadTexture(GridTexture);
    }

    void Show() override
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

        if (ImGui::Begin("3D View - Right Drag to Move", &Open, ImGuiWindowFlags_NoScrollbar))
        {
            Focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);

            ImVec2 size = ImGui::GetContentRegionAvail();

            Rectangle viewRect = { 0 };
            viewRect.x = ViewTexture.texture.width / 2 - size.x / 2;
            viewRect.y = ViewTexture.texture.height / 2 - size.y / 2;
            viewRect.width = size.x;
            viewRect.height = -size.y;

            // draw the view
            RLImGuiImageRect(&ViewTexture.texture, (int)size.x, (int)size.y, viewRect);

            ImGui::SetCursorPos(ImVec2(0, 0));

            ImGui::End();
        }
        ImGui::PopStyleVar();
    }

    void Update() override
    {
        if (!Open)
            return;

        if (IsWindowResized())
        {
            UnloadRenderTexture(ViewTexture);
            ViewTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        }

        BeginTextureMode(ViewTexture);
        ClearBackground(SKYBLUE);

        if (Focused && IsMouseButtonDown(1))
            Camera.Update();

        Camera.BeginMode3D();

        // grid of cube trees on a plane to make a "world"
        DrawPlane(Vector3 { 0, 0, 0 }, Vector2{ 50, 50 }, BEIGE); // simple world plane
        float spacing = 4;
        int count = 5;

        for (float x = -count * spacing; x <= count * spacing; x += spacing)
        {
            for (float z = -count * spacing; z <= count * spacing; z += spacing)
            {
                Vector3 pos = { x, 0.5f, z };

                Vector3 min = { x - 0.5f,0,z - 0.5f };
                Vector3 max = { x + 0.5f,1,z + 0.5f };

                DrawCubeTexture(GridTexture, Vector3{ x, 1.5f, z }, 1, 1, 1, GREEN);
                DrawCubeTexture(GridTexture, Vector3{ x, 0.5f, z }, 0.25f, 1, 0.25f, BROWN);
            }
        }

        Camera.EndMode3D();
        EndTextureMode();
 
    }

    FPCamera Camera;
    Texture2D GridTexture = { 0 };
};


ImageViewerWindow ImageViewer;
SceneViewWindow SceneView;

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
			ImGui::MenuItem("Image Viewer", nullptr, &ImageViewer.Open);
            ImGui::MenuItem("3D View", nullptr, &SceneView.Open);

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
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

	ImageViewer.Setup();
    ImageViewer.Open = true;

    SceneView.Setup();
    SceneView.Open = true;

	// Main game loop
	while (!WindowShouldClose() && !Quit)    // Detect window close button or ESC key
	{
		ImageViewer.Update();
        SceneView.Update();

		BeginDrawing();
		ClearBackground(DARKGRAY);

		BeginRLImGui();
		DoMainMenu();

		if (ImGuiDemoOpen)
			ImGui::ShowDemoWindow(&ImGuiDemoOpen);

		if (ImageViewer.Open)
            ImageViewer.Show();

        if (SceneView.Open)
            SceneView.Show();

		EndRLImGui();

		EndDrawing();
		//----------------------------------------------------------------------------------
	}
	ShutdownRLImGui();

    ImageViewer.Shutdown();
    SceneView.Shutdown();

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}