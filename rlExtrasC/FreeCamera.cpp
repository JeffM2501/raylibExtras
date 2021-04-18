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

#pragma once

#include "FreeCamera.h"

void InitFreeCamera(FreeCamera* cam, float fovy, bool zIsUp)
{
    cam->ViewCam.fovy = fovy;
    cam->ViewCam.projection = CAMERA_PERSPECTIVE;
    if (zIsUp)
    {
        cam->ViewCam.up.z = 1;
        cam->ViewCam.target.y = 1;
    }
    else
    {
        cam->ViewCam.up.y = 1;
        cam->ViewCam.target.z = 1;
    }
    cam->LastMousePos = GetMousePosition();
}

void SetFreeCameraPosition(FreeCamera* cam, Vector3& pos)
{
    Vector3 forward = Vector3Normalize(Vector3Subtract(cam->ViewCam.target, cam->ViewCam.position));
    cam->ViewCam.position = pos;
    cam->ViewCam.target = Vector3Add(cam->ViewCam.position, forward);
}

void FreeCameraLookAt(FreeCamera* cam, Vector3& target)
{
    cam->ViewCam.target = target;
    Vector3 forward = Vector3Normalize(Vector3Subtract(cam->ViewCam.target, cam->ViewCam.position));
    cam->ViewCam.target = Vector3Add(cam->ViewCam.position, forward);
}

void UpdateFreeCamera(FreeCamera* cam)
{
    float degPerPixel = cam->ViewCam.fovy / GetScreenHeight();

    Vector2 delta = Vector2Scale(Vector2Subtract(cam->LastMousePos, GetMousePosition()), degPerPixel);

    Vector3 forward = Vector3Normalize(Vector3Subtract(cam->ViewCam.target, cam->ViewCam.position));
    Vector3 right = Vector3CrossProduct(forward, cam->ViewCam.up);

    Matrix tiltMat = MatrixRotate(right, delta.y * DEG2RAD);
    forward = Vector3Transform(forward, tiltMat);

    Matrix spinMat = MatrixRotate(cam->ViewCam.up, delta.x * DEG2RAD);
    forward = Vector3Transform(forward, spinMat);

    right = Vector3CrossProduct(forward, cam->ViewCam.up);

    float forwardSpeed = 0;
    if (IsKeyDown(cam->ForwardKey))
        forwardSpeed = 1;
    else if (IsKeyDown(cam->BackwardKey))
        forwardSpeed = -1;

    float sideSpeed = 0;
    if (IsKeyDown(cam->RightKey))
        sideSpeed = 1;
    else if (IsKeyDown(cam->LeftKey))
        sideSpeed = -1;

    forwardSpeed *= GetFrameTime() * cam->MoveSpeed;
    sideSpeed *= GetFrameTime() * cam->MoveSpeed;

    cam->ViewCam.position = Vector3Add(cam->ViewCam.position, Vector3Scale(forward, forwardSpeed));
    cam->ViewCam.position = Vector3Add(cam->ViewCam.position, Vector3Scale(right, sideSpeed));

    cam->ViewCam.target = Vector3Add(cam->ViewCam.position, forward);

    cam->LastMousePos = GetMousePosition();
}
