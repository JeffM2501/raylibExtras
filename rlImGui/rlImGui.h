/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   rlImGui * basic ImGui integration
*
*   LICENSE: ZLIB
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
#include "imgui.h"
#include "IconsForkAwesome.h"
#include "IconsFontAwesome5.h"

namespace ImGuiColors
{
    inline ImVec4 Convert(::Color color)
    {
        return ImVec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    }
}

// basic API
void SetupRLImGui(bool dark);
void BeginRLImGui();
void EndRLImGui();
void ShutdownRLImGui();

// Advanced StartupAPI
void InitRLGLImGui();
void FinishRLGLImguSetup();
void ReloadImGuiFonts();

// image API
void RLImGuiImage(const Texture *image);
void RLImGuiImageSize(const Texture *image, int width, int height);
void RLImGuiImageRect(const Texture* image, int destWidth, int destHeight, Rectangle sourceRect);

// Icon Fonts
void AddRLImGuiIconFonts(float size = 12.0f, bool awesome = false);