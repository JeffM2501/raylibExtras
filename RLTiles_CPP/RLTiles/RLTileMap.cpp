/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   RLTiles * Tiled map rendering
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

#include "RLTileMap.h"


Vector2 RLTileLayer::GetDisplayLocation(int x, int y, RLTiledMapTypes mapType)
{
    if (mapType == RLTiledMapTypes::Orthographic)
        return Vector2{ static_cast<float>(x * TileWidth), static_cast<float>(y * TileHeight) };

    float halfWidth = TileWidth * 0.5f;
    float halfHeight = TileHeight * 0.5f;
    float quarterHeight = TileHeight * 0.25f;

    return Vector2{ x * halfWidth - y * halfWidth - halfWidth, y * halfHeight + (x * halfHeight) };
}

RLTile RLTileMap::GetTile(int x, int y, int layerID)
{
    std::map<int, RLTileLayer>::iterator itr = Layers.find(layerID);
    if (itr == Layers.end())
        return RLTile();

    RLTileLayer &layer = itr->second;

    if (x < 0 || x >= layer.Width || y < 0 || y >= layer.Height)
        return RLTile();

    int index = y * layer.Width + x;
    return layer.Tiles[index];
}
