/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   FreeCamC * Free flight sim style camera (C version)
*
*   LICENSE: MIT
*
*   Copyright (c) 2021 Jeffery Myers
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


#ifndef FREE_CAMERA_C_H
#define FREE_CAMERA_C_H

#include "raylib.h"
#include "raymath.h"

typedef struct
{
    Vector3 Position;
    Vector3 Forward;
    Vector3 Up;
}FreeCamera;

// called to initialize a camera to default values
RLAPI void FreeCam_Init(FreeCamera* camera, Vector3 position, Vector3 up);

// called to fill out a raylib camera
RLAPI void FreeCam_ToCamera(FreeCamera* camera, Camera3D* raylibCamera);

// called to set the target the camera should look at
RLAPI void FreeCam_LookAt(FreeCamera* camera, Vector3 target, Vector3 up);

// called to set where the camera is
RLAPI void FreeCam_SetPosition(FreeCamera* camera, Vector3 position);

// Called to move the camera relative to it's current orientation
RLAPI void FreeCam_MoveForward(FreeCamera* camera, float distance);
RLAPI void FreeCam_MoveUp(FreeCamera* camera, float distance);
RLAPI void FreeCam_MoveRight(FreeCamera* camera, float distance);

// Called to rotate the camera relative to it's current orientation
RLAPI void FreeCam_RotateYaw(FreeCamera* camera, float angle);
RLAPI void FreeCam_RotatePitch(FreeCamera* camera, float angle);
RLAPI void FreeCam_RotateRoll(FreeCamera* camera, float angle);

// called to rotate the camera around a world axis (Y or Z)
RLAPI void FreeCam_RotateHeading(FreeCamera* camera, float angle, bool useY);

#endif //FREE_CAMERA_C_H
