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

#include "raylib.h"
#include "raymath.h"

class FreeCamera
{
public:
    Camera ViewCam = { 0 };
    Vector2 LastMousePos = { 0 };

    float MoveSpeed = 10;

    int ForwardKey = KEY_W;
    int BackwardKey = KEY_S;
    int LeftKey = KEY_A;
    int RightKey = KEY_D;

    inline void Init(float fovy, bool zIsUp = true)
    {
        ViewCam.fovy = fovy;
        ViewCam.projection = CAMERA_PERSPECTIVE;
        if (zIsUp)
        {
            ViewCam.up.z = 1;
            ViewCam.target.y = 1;
        }
        else
        {
            ViewCam.up.y = 1;
            ViewCam.target.z = 1;
        }
        LastMousePos = GetMousePosition();
    }

    inline void SetPosition(Vector3& pos)
    {
        Vector3 forward = Vector3Normalize(Vector3Subtract(ViewCam.target, ViewCam.position));
        ViewCam.position = pos;
        ViewCam.target = Vector3Add(ViewCam.position, forward);
    }

    inline void LookAt(Vector3& target)
    {
        ViewCam.target = target;
        Vector3 forward = Vector3Normalize(Vector3Subtract(ViewCam.target, ViewCam.position));
        ViewCam.target = Vector3Add(ViewCam.position, forward);
    }

    inline void Update()
    {
        float degPerPixel = ViewCam.fovy / GetScreenHeight();

        Vector2 delta = Vector2Scale(Vector2Subtract(LastMousePos, GetMousePosition()), degPerPixel);

        Vector3 forward = Vector3Normalize(Vector3Subtract(ViewCam.target, ViewCam.position));
        Vector3 right = Vector3CrossProduct(forward, ViewCam.up);

        Matrix tiltMat = MatrixRotate(right, delta.y * DEG2RAD);
        forward = Vector3Transform(forward, tiltMat);

        Matrix spinMat = MatrixRotate(ViewCam.up, delta.x * DEG2RAD);
        forward = Vector3Transform(forward, spinMat);

        right = Vector3CrossProduct(forward, ViewCam.up);

        float forwardSpeed = 0;
        if (IsKeyDown(ForwardKey))
            forwardSpeed = 1;
        else if (IsKeyDown(BackwardKey))
            forwardSpeed = -1;

        float sideSpeed = 0;
        if (IsKeyDown(RightKey))
            sideSpeed = 1;
        else if (IsKeyDown(LeftKey))
            sideSpeed = -1;

        forwardSpeed *= GetFrameTime() * MoveSpeed;
        sideSpeed *= GetFrameTime() * MoveSpeed;

        ViewCam.position = Vector3Add(ViewCam.position, Vector3Scale(forward, forwardSpeed));
        ViewCam.position = Vector3Add(ViewCam.position, Vector3Scale(right, sideSpeed));

        ViewCam.target = Vector3Add(ViewCam.position, forward);

        LastMousePos = GetMousePosition();
    }
};