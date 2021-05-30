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

#include <cmath>
#include "RLFrustum.h"
#include "rlgl.h"

void NormalizePlane(Vector4& plane)
{
    float magnitude = sqrtf(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);

    plane.x /= magnitude;
    plane.y /= magnitude;
    plane.z /= magnitude;
    plane.w /= magnitude;
}

RLFrustum::RLFrustum()
{
    Planes[FrustumPlanes::Right] = Vector4{ 0 };
    Planes[FrustumPlanes::Left] = Vector4{ 0 };
    Planes[FrustumPlanes::Top] = Vector4{ 0 };
    Planes[FrustumPlanes::Bottom] = Vector4{ 0 };
    Planes[FrustumPlanes::Front] = Vector4{ 0 };
    Planes[FrustumPlanes::Back] = Vector4{ 0 };
}

void RLFrustum::Extract()
{
    Matrix projection = rlGetMatrixProjection();
    Matrix modelview = rlGetMatrixModelview();

    Matrix planes = { 0 };

    planes.m0 = modelview.m0 * projection.m0 + modelview.m1 * projection.m4 + modelview.m2 * projection.m8 + modelview.m3 * projection.m12;
    planes.m1 = modelview.m0 * projection.m1 + modelview.m1 * projection.m5 + modelview.m2 * projection.m9 + modelview.m3 * projection.m13;
    planes.m2 = modelview.m0 * projection.m2 + modelview.m1 * projection.m6 + modelview.m2 * projection.m10 + modelview.m3 * projection.m14;
    planes.m3 = modelview.m0 * projection.m3 + modelview.m1 * projection.m7 + modelview.m2 * projection.m11 + modelview.m3 * projection.m15;
    planes.m4 = modelview.m4 * projection.m0 + modelview.m5 * projection.m4 + modelview.m6 * projection.m8 + modelview.m7 * projection.m12;
    planes.m5 = modelview.m4 * projection.m1 + modelview.m5 * projection.m5 + modelview.m6 * projection.m9 + modelview.m7 * projection.m13;
    planes.m6 = modelview.m4 * projection.m2 + modelview.m5 * projection.m6 + modelview.m6 * projection.m10 + modelview.m7 * projection.m14;
    planes.m7 = modelview.m4 * projection.m3 + modelview.m5 * projection.m7 + modelview.m6 * projection.m11 + modelview.m7 * projection.m15;
    planes.m8 = modelview.m8 * projection.m0 + modelview.m9 * projection.m4 + modelview.m10 * projection.m8 + modelview.m11 * projection.m12;
    planes.m9 = modelview.m8 * projection.m1 + modelview.m9 * projection.m5 + modelview.m10 * projection.m9 + modelview.m11 * projection.m13;
    planes.m10 = modelview.m8 * projection.m2 + modelview.m9 * projection.m6 + modelview.m10 * projection.m10 + modelview.m11 * projection.m14;
    planes.m11 = modelview.m8 * projection.m3 + modelview.m9 * projection.m7 + modelview.m10 * projection.m11 + modelview.m11 * projection.m15;
    planes.m12 = modelview.m12 * projection.m0 + modelview.m13 * projection.m4 + modelview.m14 * projection.m8 + modelview.m15 * projection.m12;
    planes.m13 = modelview.m12 * projection.m1 + modelview.m13 * projection.m5 + modelview.m14 * projection.m9 + modelview.m15 * projection.m13;
    planes.m14 = modelview.m12 * projection.m2 + modelview.m13 * projection.m6 + modelview.m14 * projection.m10 + modelview.m15 * projection.m14;
    planes.m15 = modelview.m12 * projection.m3 + modelview.m13 * projection.m7 + modelview.m14 * projection.m11 + modelview.m15 * projection.m15;

    Planes[FrustumPlanes::Right] = { planes.m3 - planes.m0, planes.m7 - planes.m4, planes.m11 - planes.m8, planes.m15 - planes.m12 };
    NormalizePlane(Planes[FrustumPlanes::Right]);

    Planes[FrustumPlanes::Left] = { planes.m3 + planes.m0, planes.m7 + planes.m4, planes.m11 + planes.m8, planes.m15 + planes.m12 };
    NormalizePlane(Planes[FrustumPlanes::Left]);

    Planes[FrustumPlanes::Top] = { planes.m3 - planes.m1, planes.m7 - planes.m5, planes.m11 - planes.m9, planes.m15 - planes.m13 };
    NormalizePlane(Planes[FrustumPlanes::Top]);

    Planes[FrustumPlanes::Bottom] = { planes.m3 + planes.m1, planes.m7 + planes.m5, planes.m11 + planes.m9, planes.m15 + planes.m13 };
    NormalizePlane(Planes[FrustumPlanes::Bottom]);

    Planes[FrustumPlanes::Back] = { planes.m3 - planes.m2, planes.m7 - planes.m6, planes.m11 - planes.m10, planes.m15 - planes.m14 };
    NormalizePlane(Planes[FrustumPlanes::Back]);

    Planes[FrustumPlanes::Front] = { planes.m3 + planes.m2, planes.m7 + planes.m6, planes.m11 + planes.m10, planes.m15 + planes.m14 };
    NormalizePlane(Planes[FrustumPlanes::Front]);
}

float DistanceToPlane(const Vector4& plane, const Vector3& position)
{
    return (plane.x * position.x + plane.y * position.y + plane.z * position.z + plane.w);
}

float DistanceToPlane(const Vector4& plane, float x, float y, float z)
{
    return (plane.x * x + plane.y * y + plane.z * z + plane.w);
}

bool RLFrustum::PointIn(const Vector3& position) const
{
    for (auto& plane : Planes)
    {
        if (DistanceToPlane(plane.second, position) <= 0) // point is behind plane
            return false;
    }

    return true;
}

bool RLFrustum::PointIn(float x, float y, float z) const
{
    for (auto& plane : Planes)
    {
        if (DistanceToPlane(plane.second, x, y, z) <= 0) // point is behind plane
            return false;
    }

    return true;
}

bool RLFrustum::SphereIn(const Vector3& position, float radius) const
{
    for (auto& plane : Planes)
    {
        if (DistanceToPlane(plane.second, position) < -radius) // center is behind plane by more than the radius
            return false;
    }

    return true;
}

bool RLFrustum::AABBoxIn(const Vector3& min, const Vector3& max) const
{
    // if any point is in and we are good
    if (PointIn(min.x, min.y, min.z))
        return true;

    if (PointIn(min.x, max.y, min.z))
        return true;

    if (PointIn(max.x, max.y, min.z))
        return true;

    if (PointIn(max.x, min.y, min.z))
        return true;

    if (PointIn(min.x, min.y, max.z))
        return true;

    if (PointIn(min.x, max.y, max.z))
        return true;

    if (PointIn(max.x, max.y, max.z))
        return true;

    if (PointIn(max.x, min.y, max.z))
        return true;

    // check to see if all points are outside of any one plane, if so the entire box is outside
    for (auto& plane : Planes)
    {
        bool oneInside = false;

        if (DistanceToPlane(plane.second, min.x, min.y, min.z) >= 0)
            oneInside = true;

        if (DistanceToPlane(plane.second, max.x, min.y, min.z) >= 0)
            oneInside = true;

        if (DistanceToPlane(plane.second, max.x, max.y, min.z) >= 0)
            oneInside = true;

        if (DistanceToPlane(plane.second, min.x, max.y, min.z) >= 0)
            oneInside = true;

        if (DistanceToPlane(plane.second, min.x, min.y, max.z) >= 0)
            oneInside = true;

        if (DistanceToPlane(plane.second, max.x, min.y, max.z) >= 0)
            oneInside = true;

        if (DistanceToPlane(plane.second, max.x, max.y, max.z) >= 0)
            oneInside = true;

        if (DistanceToPlane(plane.second, min.x, max.y, max.z) >= 0)
            oneInside = true;

        if (!oneInside)
            return false;
    }

    // the box extends outside the frustum but crosses it
    return true;
}