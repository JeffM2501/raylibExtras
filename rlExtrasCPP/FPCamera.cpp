/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   RLAssets * Simple Asset Managment System for Raylib
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

#include "FPCamera.h"

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

#include <cmath>

FPCamera::FPCamera() : ControlsKeys{ 'W', 'S', 'D', 'A', 'E', 'Q', KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_LEFT_SHIFT }
{

}

void FPCamera::Setup(float fovY, Vector3&& position)
{
    CameraPosition = position;
    ViewCamera.position = { position.x, position.y, position.z };
    ViewCamera.position.y += PlayerEyesPosition;
    ViewCamera.target = Vector3Add(ViewCamera.position, Vector3{ 0, 0, 1 });
    ViewCamera.up = { 0.0f, 1.0f, 0.0f };
    ViewCamera.fovy = fovY;
    ViewCamera.projection = CAMERA_PERSPECTIVE;

    Focused = IsWindowFocused();
    if (Focused && (UseMouseX || UseMouseY))
        DisableCursor();

    TargetDistance = 1;

    ViewResized();
    PreviousMousePosition = GetMousePosition();
}

void FPCamera::ViewResized()
{
    float width = (float)GetScreenWidth();
    float height = (float)GetScreenHeight();

    FOV.y = ViewCamera.fovy;

    if (height != 0)
        FOV.x = FOV.y * (width / height);
}

float FPCamera::GetSpeedForAxis(CameraControls axis, float speed)
{
    int key = ControlsKeys[axis];
    if (key == -1)
        return 0;

    float factor = 1.0f;
    if (IsKeyDown(ControlsKeys[SPRINT]))
        factor = 2;

    if (IsKeyDown(ControlsKeys[axis]))
        return speed * GetFrameTime() * factor;

    return 0.0f;
}

void FPCamera::BeginMode3D()
{
    float aspect = (float)GetScreenWidth() / (float)GetScreenHeight();

    rlDrawRenderBatchActive();			// Draw Buffers (Only OpenGL 3+ and ES2)
    rlMatrixMode(RL_PROJECTION);        // Switch to projection matrix
    rlPushMatrix();                     // Save previous matrix, which contains the settings for the 2d ortho projection
    rlLoadIdentity();                   // Reset current matrix (projection)

    if (ViewCamera.projection == CAMERA_PERSPECTIVE)
    {
        // Setup perspective projection
        double top = RL_CULL_DISTANCE_NEAR * tan(ViewCamera.fovy * 0.5 * DEG2RAD);
        double right = top * aspect;

        rlFrustum(-right, right, -top, top, NearPlane, FarPlane);
    }
    else if (ViewCamera.projection == CAMERA_ORTHOGRAPHIC)
    {
        // Setup orthographic projection
        double top = ViewCamera.fovy / 2.0;
        double right = top * aspect;

        rlOrtho(-right, right, -top, top, NearPlane, FarPlane);
    }

    // NOTE: zNear and zFar values are important when computing depth buffer values

    rlMatrixMode(RL_MODELVIEW);         // Switch back to modelview matrix
    rlLoadIdentity();                   // Reset current matrix (modelview)

    // Setup Camera view
    Matrix matView = MatrixLookAt(ViewCamera.position, ViewCamera.target, ViewCamera.up);
    rlMultMatrixf(MatrixToFloat(matView));      // Multiply modelview matrix by view matrix (camera)

    rlEnableDepthTest();                // Enable DEPTH_TEST for 3D
    ExtractFrustum();
}

void FPCamera::EndMode3D()
{
    ::EndMode3D();
}

void FPCamera::Update()
{
    if (IsWindowFocused() != Focused && (UseMouseX || UseMouseY))
    {
        Focused = IsWindowFocused();
        if (Focused)
        {
            DisableCursor();
            PreviousMousePosition = GetMousePosition(); // so there is no jump on focus
        }
        else
        {
            EnableCursor();
        }
    }

    // Mouse movement detection
    Vector2 mousePositionDelta = { 0.0f, 0.0f };
    Vector2 mousePosition = GetMousePosition();
    float mouseWheelMove = GetMouseWheelMove();

    // Keys input detection
    float direction[MOVE_DOWN+1] = {  GetSpeedForAxis(MOVE_FRONT,MoveSpeed.z),
                                      GetSpeedForAxis(MOVE_BACK,MoveSpeed.z),
                                      GetSpeedForAxis(MOVE_RIGHT,MoveSpeed.x),
                                      GetSpeedForAxis(MOVE_LEFT,MoveSpeed.x),
                                      GetSpeedForAxis(MOVE_UP,MoveSpeed.y),
                                      GetSpeedForAxis(MOVE_DOWN,MoveSpeed.y) };

    mousePositionDelta.x = mousePosition.x - PreviousMousePosition.x;
    mousePositionDelta.y = mousePosition.y - PreviousMousePosition.y;

    PreviousMousePosition = mousePosition;

    Vector3 forward = Vector3Subtract(ViewCamera.target,ViewCamera.position);
    forward.y = 0;
    forward = Vector3Normalize(forward);

    Vector3 right{ forward.z * -1.0f, 0, forward.x };

    Vector3 oldPosition = CameraPosition;

    CameraPosition = Vector3Add(CameraPosition, Vector3Scale(forward, direction[MOVE_FRONT] - direction[MOVE_BACK]));
    CameraPosition = Vector3Add(CameraPosition, Vector3Scale(right, direction[MOVE_RIGHT] - direction[MOVE_LEFT]));

    CameraPosition.y += direction[MOVE_UP] - direction[MOVE_DOWN];

    // let someone modify the projected position
    if (ValidateCamPosition != nullptr)
        ValidateCamPosition(*this, CameraPosition, oldPosition);

    // Camera orientation calculation
    float turnRotation = GetSpeedForAxis(TURN_RIGHT, TurnSpeed.x) - GetSpeedForAxis(TURN_LEFT, TurnSpeed.x);
    float tiltRotation = GetSpeedForAxis(TURN_UP, TurnSpeed.y) - GetSpeedForAxis(TURN_DOWN, TurnSpeed.y);

    if (turnRotation != 0)
        Angle.x -= turnRotation * DEG2RAD;
    else if (UseMouseX && Focused)
        Angle.x += (mousePositionDelta.x / -MouseSensitivity);
    
    if (tiltRotation)
        Angle.y += tiltRotation * DEG2RAD;
    else if (UseMouseY && Focused)
        Angle.y += (mousePositionDelta.y / -MouseSensitivity);

        // Angle clamp
    if (Angle.y < MinimumViewY * DEG2RAD)
        Angle.y = MinimumViewY * DEG2RAD;
    else if (Angle.y > MaximumViewY * DEG2RAD) 
       Angle.y = MaximumViewY * DEG2RAD;

        // Recalculate camera target considering translation and rotation
    Vector3 target = Vector3Transform(Vector3{ 0, 0, 1 }, MatrixRotateXYZ(Vector3{ Angle.y, -Angle.x, 0 }));

    ViewCamera.position = CameraPosition;

    float eyeOfset = PlayerEyesPosition;

    if (ViewBobbleFreq > 0)
    {
        float swingDelta = std::fmax(std::abs(direction[MOVE_FRONT] - direction[MOVE_BACK]), std::abs(direction[MOVE_RIGHT] - direction[MOVE_LEFT]));

        // If movement detected (some key pressed), increase swinging
        CurrentBobble += swingDelta * ViewBobbleFreq;

        constexpr float ViewBobbleDampen = 8.0f;

        eyeOfset -= sinf(CurrentBobble / ViewBobbleDampen) * ViewBobbleMagnatude;

        ViewCamera.up.x = sinf(CurrentBobble / (ViewBobbleDampen * 2)) * ViewBobbleWaverMagnitude;
        ViewCamera.up.z = -sinf(CurrentBobble / (ViewBobbleDampen * 2)) * ViewBobbleWaverMagnitude;
    }
    else
    {
        CurrentBobble = 0;
        ViewCamera.up.x = 0;
        ViewCamera.up.z = 0;
    }

    ViewCamera.position.y += eyeOfset;

    ViewCamera.target.x = ViewCamera.position.x + target.x;
    ViewCamera.target.y = ViewCamera.position.y + target.y;
    ViewCamera.target.z = ViewCamera.position.z + target.z;
}

float FPCamera::GetFOVX() const
{
    return FOV.x;
}

Vector3 FPCamera::GetCameraPosition() const
{
    return CameraPosition;
}
