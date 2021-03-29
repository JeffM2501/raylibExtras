/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   RLGeoTools * Geometry extensions to raylib
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

#ifndef RLGEOTOOLS_H
#define RLGEOTOOLS_H

#include "raylib.h"															// Required for structs: Vector3, Matrix
#include <math.h>

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

RLAPI Shader SetModelMaterialShader(Model* model, int materialIndex, Shader shader);
RLAPI void SetModelMaterialShaderValue(Model* model, int materialIndex, const char* location, const void* value, int uniformType);
RLAPI void SetModelMaterialShaderValueV(Model* model, int materialIndex, const char* location, const void* value, int uniformType, int count);
RLAPI void SetModelMaterialTexture(Model* model, int materialIndex, int maptype, Texture2D texture);

RLAPI Shader LoadShaderSet(const char* resourcePath, const char* name);
RLAPI Shader LoadShaders(const char* resourcePath, const char* vsName, const char* fsName);

// Custom mesh callback type
typedef void (*MeshGenerateCallback)(Mesh* mesh, void* userData);

/// <summary>
/// Merges an array of models into a single model with all the meshes from the single model.
/// Note that this function will move the meshes out of the input models into the output model
/// The input models will be unloaded and no longer valid
/// </summary>
/// <param name="models">an array of models</param>
/// <param name="count">the size of the array of models</param>
/// <returns>a new model with all meshes</returns>
Model MergeModels(Model* models, size_t count);

/// <summary>
/// Burns the model transform into the mesh geometry and clears the model transform
/// Useful before doing MergeModels 
/// </summary>
/// <param name="model">pointer to the model to modify</param>
void ApplyModelTransformToMeshes(Model* model);

/// <summary>
/// Generate custom mesh
/// </summary>
/// <param name="generate">callback function that is called with the mesh to be created</param>
/// <param name="userData">arbitary user data passed to the callback</param>
/// <returns>The completed mesh</returns>
RLAPI Mesh GenMeshCustomCallback(MeshGenerateCallback generate, void* userData); 

/// <summary>
/// Allocate mesh data based on passed in counts
/// </summary>
/// <param name="mesh">The mesh to fill out</param>
/// <param name="verts">The number of vertecies</param>
/// <param name="triangles">The number of triangles</param>
/// <param name="colors">The number of vertex colors</param>
/// <param name="uvs">The number of UV coordianates</param>
/// <param name="uv2s">The Number of </param>
RLAPI void AllocateMeshData(Mesh* mesh, int verts, int triangles, int colors, bool uvs, bool uv2s);	

/// <summary>
/// Renders the contents of a Vector3 to the screen for easy debugging
/// </summary>
/// <param name="vector">The value to output</param>
/// <param name="posX">Screen position in X</param>
/// <param name="posY">Screen position in Y</param>
/// <param name="rightJustify">If true, posX specifies the right side of the text, if false the left side</param>
RLAPI void DrawVector3Text(Vector3* vector, int posX, int posY, bool rightJustify);

/// <summary>
/// Renders the contents of a Vector2 to the screen for easy debugging
/// </summary>
/// <param name="vector">The value to output</param>
/// <param name="posX">Screen position in X</param>
/// <param name="posY">Screen position in Y</param>
/// <param name="rightJustify">If true, posX specifies the right side of the text, if false the left side</param>
RLAPI void DrawVector2Text(Vector2* vector, int posX, int posY, bool rightJustify);

/// <summary>
/// Draws a 3d line in a color, from floats
/// </summary>
/// <param name="spx">start point X</param>
/// <param name="spy">start point Y</param>
/// <param name="spz">start point Z</param>
/// <param name="epx">end point X</param>
/// <param name="epy">end point Y</param>
/// <param name="epz">end point Z</param>
/// <param name="color">The color to use</param>
RLAPI void DrawLine3DF(float spx, float spy, float spz, float epx, float epy, float epz, Color color);

/// <summary>
/// Draws a wire frame rectangle in XZ in 3D Space
/// </summary>
/// <param name="rect">The rectangle to draw</param>
/// <param name="y">The Y height to draw it at</param>
/// <param name="color">The color to use</param>
RLAPI void DrawRect3DXZ(Rectangle rect, float y, Color color);

/// <summary>
/// Draws a 2D circle to the screen using floating point values
/// </summary>
/// <param name="centerX">Center In X</param>
/// <param name="centerY">Center in Y</param>
/// <param name="radius">Radius of circle</param>
/// <param name="color">Color to use</param>
RLAPI void DrawCircleF(float centerX, float centerY, float radius, Color color);

/// <summary>
/// Draws a 2D line using floating point values
/// </summary>
/// <param name="startPosX"></param>
/// <param name="startPosY"></param>
/// <param name="endPosX"></param>
/// <param name="endPosY"></param>
/// <param name="color">Color to use</param>
/// <returns></returns>
RLAPI void DrawLineF(float startPosX, float startPosY, float endPosX, float endPosY, Color color);

/// <summary>
/// Draws a 2D rectangle using floating point values
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="color">Colr to use</param>
RLAPI void DrawRectangleF(float posX, float posY, float width, float height, Color color);

/// <summary>
/// Integer based vector
/// </summary>
typedef struct Vector2i
{
    int x;
    int y;
 } Vector2i;

// Subtract two vectors (v1 - v2)
inline RLAPI Vector2i Vector2ISubtract(Vector2i v1, Vector2i v2)
{
    Vector2i result = { v1.x - v2.x, v1.y - v2.y };
    return result;
}

// Calculate vector length
inline RLAPI float Vector2ILength(Vector2i v)
{
    float result = sqrtf((float)((v.x * v.x) + (v.y * v.y)));
    return result;
}


#if defined(__cplusplus)
}
#endif


#endif  // RLGEOTOOLS_H
