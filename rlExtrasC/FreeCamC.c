/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   FreeCamC * Free flight sim style camera (C version)
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


#include "FreeCamC.h"
#include <stdlib.h>
#include <math.h>


// called to initialize a camera to default values
void FreeCam_Init(FreeCamera* camera, Vector3 position, Vector3 up)
{
    camera->Position = position;
    camera->Up = up;
    if (fabs(camera->Up.z) > 0.1f)
        camera->Forward = (Vector3){ 0, 1, 0 };
    else
        camera->Forward = (Vector3){ 0, 0, 1 };
}

// called to fill out a raylib camera
void FreeCam_ToCamera(FreeCamera* camera, Camera3D* raylibCamera)
{
    raylibCamera->position = camera->Position;
    raylibCamera->target = Vector3Add(camera->Position, camera->Forward);
    raylibCamera->up = camera->Up;
}

// called to set the target the camera should look at
void FreeCam_LookAt(FreeCamera* camera, Vector3 target, Vector3 up)
{
    camera->Forward = Vector3Normalize(Vector3Subtract(target, camera->Position));
    camera->Up = Vector3Normalize(up);
}

// called to set where the camera is
void FreeCam_SetPosition(FreeCamera* camera, Vector3 position)
{
    camera->Position = camera->Position;
}

// Called to move the camera relative to it's current orientation
void FreeCam_MoveForward(FreeCamera* camera, float distance)
{
    camera->Position = Vector3Add(camera->Position, Vector3Scale(camera->Forward, distance));
}

void FreeCam_MoveUp(FreeCamera* camera, float distance)
{
    camera->Position = Vector3Add(camera->Position, Vector3Scale(camera->Up, distance));
}

Vector3 GetRightVector(FreeCamera* camera)
{
    return Vector3CrossProduct(camera->Forward, camera->Up);
}

void FreeCam_MoveRight(FreeCamera* camera, float distance)
{
    camera->Position = Vector3Add(camera->Position, Vector3Scale(GetRightVector(camera), distance));
}

// Called to rotate the camera relative to it's current orientation
void FreeCam_RotateYaw(FreeCamera* camera, float angle)
{
    Matrix matrix = MatrixRotate(camera->Up, DEG2RAD * angle);
    camera->Forward = Vector3Normalize(Vector3Transform(camera->Forward, matrix));
}

void FreeCam_RotatePitch(FreeCamera* camera, float angle)
{
    angle = fmodf(angle, 360);
    if (angle < 0)
        angle += 360;

    Matrix matrix = MatrixRotate(GetRightVector(camera), DEG2RAD * -angle);

    camera->Up = Vector3Normalize(Vector3Transform(camera->Up, matrix));
    camera->Forward = Vector3Normalize(Vector3Transform(camera->Forward, matrix));
}

void FreeCam_RotateRoll(FreeCamera* camera, float angle)
{
    Matrix matrix = MatrixRotate(camera->Forward, DEG2RAD * angle);
    camera->Up = Vector3Normalize(Vector3Transform(camera->Up, matrix));
}

// called to rotate the camera around a world axis (Y or Z)
void FreeCam_RotateHeading(FreeCamera* camera, float angle, bool useY)
{
    Matrix matrix;
    if (useY)
        matrix = MatrixRotateY(DEG2RAD * angle);
    else
        matrix = MatrixRotateZ(DEG2RAD * angle);

    camera->Up = Vector3Normalize(Vector3Transform(camera->Up, matrix));
    camera->Forward = Vector3Normalize(Vector3Transform(camera->Forward, matrix));
}