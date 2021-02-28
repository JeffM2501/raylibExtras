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

#include "RLGeoTools.h"
#include "utils.h"
#include "rlgl.h"

#include <stdlib.h>         // Required for: malloc(), free()
#include <stdio.h>         // Required for: sprintf()

Shader SetModelMaterialShader(Model* model, int materialIndex, Shader shader)
{
	model->materials[materialIndex].shader = shader;
	return shader;
}

void SetModelMaterialShaderValue(Model* model, int materialIndex, const char* location, const void* value, int uniformType)
{
	Shader shader = model->materials[materialIndex].shader;
	SetShaderValue(shader, GetShaderLocation(shader, location), value, uniformType);
}

void SetModelMaterialShaderValueV(Model* model, int materialIndex, const char* location, const void* value, int uniformType, int count)
{
	Shader shader = model->materials[materialIndex].shader;
	SetShaderValueV(shader, GetShaderLocation(shader, location), value, uniformType, count);
}

void SetModelMaterialTexture(Model* model, int materialIndex, int maptype, Texture2D texture)
{
	SetMaterialTexture(&model->materials[materialIndex], maptype, texture);
}

RLAPI Shader LoadShaderSet(const char* resourcePath, const char* name)
{
	static char vsTemp[512];
	static char fsTemp[512];

#if defined(PLATFORM_DESKTOP)
	static const char* glsl = "glsl110";
#else
	static const char* glsl = "glsl330";
#endif
	sprintf(vsTemp, "%s/%s/%s.vs", resourcePath, glsl, name);
	sprintf(fsTemp, "%s/%s/%s.fs", resourcePath, glsl, name);

	return LoadShader(vsTemp, fsTemp);
}

RLAPI Shader LoadShaders(const char* resourcePath, const char* vsName, const char* fsName)
{
	static char vsTemp[512];
	static char fsTemp[512];

#if defined(PLATFORM_DESKTOP)
	static const char* glsl = "glsl110";
#else
	static const char* glsl = "glsl330";
#endif
	sprintf(vsTemp, "%s/%s/%s.vs", resourcePath, glsl, vsName);
	sprintf(fsTemp, "%s/%s/%s.fs", resourcePath, glsl, fsName);

	return LoadShader(vsTemp, fsTemp);
}

#ifndef DEFAULT_MESH_VERTEX_BUFFERS
#define DEFAULT_MESH_VERTEX_BUFFERS    7    // Number of vertex buffers (VBO) per mesh
#endif

void AllocateMeshData(Mesh* mesh, int verts, int triangles, int colors, bool uvs, bool uv2s)
{
	mesh->vboId = (unsigned int*)MemAlloc(DEFAULT_MESH_VERTEX_BUFFERS *sizeof(unsigned int));
	for (int i = 0; i < DEFAULT_MESH_VERTEX_BUFFERS; i++)
		mesh->vboId[i] = 0;

	mesh->vertices = (float*)MemAlloc(verts * 3 * sizeof(float));
	mesh->normals = (float*)MemAlloc(verts * 3 * sizeof(float));

	if (uvs)
		mesh->texcoords = (float*)MemAlloc(verts * 2 * sizeof(float));
	else
		mesh->texcoords = NULL;

	if (uv2s)
		mesh->texcoords2 = (float*)MemAlloc(verts * 2 * sizeof(float));
	else
		mesh->texcoords2 = NULL;

	mesh->indices = (unsigned short*)MemAlloc(triangles * 3 * sizeof(unsigned short));
	mesh->vertexCount = verts;
	mesh->triangleCount = triangles;
}

Mesh GenMeshCustom(MeshGenerateCallback generate, void* userData)
{
	Mesh mesh = { 0 };
	
	if (generate != NULL)   // ask the callback to generate the mesh data
		generate(&mesh, userData);

	// Upload vertex data to GPU (static mesh)
	rlLoadMesh(&mesh, false);

	return mesh;
}

void DrawVector3Text(Vector3* vector, int posX, int posY, bool rightJustify)
{
	const int size = 20;
	const char* text = TextFormat("%.2f,%.2f,%.2f", vector->x, vector->y, vector->z);

	int offset = 0;
	if (rightJustify)
		offset = MeasureText(text, size);
	DrawText(text, posX - offset, posY, size, LIME);
}

void DrawVector2Text(Vector2* vector, int posX, int posY, bool rightJustify)
{
	const int size = 20;
	const char* text = TextFormat("%.2f,%.2f", vector->x, vector->y);

	int offset = 0;
	if (rightJustify)
		offset = MeasureText(text, size);
	DrawText(text, posX - offset, posY, size, LIME);
}

void DrawLine3DF(float spx, float spy, float spz, float epx, float epy, float epz, Color color)
{
	rlBegin(RL_LINES);
	rlColor4ub(color.r, color.g, color.b, color.a);
	rlVertex3f(spx, spy, spz);
	rlVertex3f(epx, epy, epz);
	rlEnd();
}

void DrawRect3DXZ(Rectangle rect, float y, Color color)
{
	rlBegin(RL_LINES);
	rlColor4ub(color.r, color.g, color.b, color.a);
	rlVertex3f(rect.x, y, rect.y);
	rlVertex3f(rect.x + rect.width, y, rect.y);

	rlVertex3f(rect.x + rect.width, y, rect.y);
 	rlVertex3f(rect.x + rect.width, y, rect.y + rect.height);

	rlVertex3f(rect.x + rect.width, y, rect.y + rect.height);
 	rlVertex3f(rect.x, y, rect.y + rect.height);
	
	rlVertex3f(rect.x, y, rect.y + rect.height);
	rlVertex3f(rect.x, y, rect.y);
	rlEnd();
}

void DrawCircleF(float centerX, float centerY, float radius, Color color)
{
	DrawCircleV((Vector2) { centerX, centerY }, radius, color);
}

void DrawLineF(float startPosX, float startPosY, float endPosX, float endPosY, Color color)
{
	rlBegin(RL_LINES);
	rlColor4ub(color.r, color.g, color.b, color.a);
	rlVertex2f(startPosX, startPosY);
	rlVertex2f(endPosX, endPosY);
	rlEnd();
}

void DrawRectangleF(float posX, float posY, float width, float height, Color color)
{
	DrawRectangleV((Vector2) { posX, posY }, (Vector2) { width, height }, color);
}