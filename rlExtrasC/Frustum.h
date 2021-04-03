/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   RLSprite * Simple Sprite Managment System for Raylib
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
#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "raylib.h"
#include "raymath.h"

typedef enum 
{
    Back = 0,
    Front = 1, 
    Bottom = 2,
    Top = 3,
	Right = 4,
	Left = 5,
	MAX = 6
}FrustumPlanes;

typedef struct 
{
	Vector4 Planes[6];
}Frustum;

RLAPI void ExtractFrustum(Frustum* frustrum);
RLAPI bool PointInFrustumV(Frustum* frustrum, Vector3 position);
RLAPI bool SphereInFrustumV(Frustum* frustrum, Vector3 position, float radius);

RLAPI bool AABBoxInFrustum(Frustum* frustrum, Vector3 min, Vector3 max);

#endif //FRUSTUM_H