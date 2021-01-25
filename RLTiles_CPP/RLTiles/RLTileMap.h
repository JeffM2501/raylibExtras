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


#ifndef RLTILEMAP_H
#define RLTILEMAP_H

#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <map>
#include <string>
#include <memory>

class RLTileSheet
{
public:
    int ID = 0;

    std::string SheetSource;
    int StartFrame = 0;

    std::vector<Rectangle> Tiles;

    inline Rectangle GetFrame(int tileID)
    {
        int index = tileID - StartFrame;
        if (index >= 0 && index < Tiles.size())
            return Tiles[index];

        return Rectangle{ 0,0,0,0 };
    }
};

enum class RLTiledMapTypes
{
    Orthographic,
    Isometric,
};

typedef struct RLTile
{
    bool FlipX = false;
    bool FilpY = false;
    bool FlipDiag = false;

    int16_t TileID = -1;
}RLTile;

class RLLayer
{
public:
    enum class LayerTypes
    {
        Tile,
        Object,
        Image,
    };

    LayerTypes LayerType = LayerTypes::Tile;

    int ID = 0;
    std::string Name;
    Rectangle Bounds = { 0,0,0,0 };
    Vector2 Offset = { 0,0 };
    bool Visible = true;

    Color Tint = WHITE;

    virtual~RLLayer() {}
};

class RLTileLayer : public RLLayer
{
public:
    Vector2 TileSize = { 0,0 };
    std::vector<RLTile> Tiles;

    typedef std::shared_ptr<RLTileLayer> Ptr;
};

class RLTileObject 
{
public:
    int ID = 0;
    std::string Name;
    Rectangle Bounds = { 0,0,0,0 };

    bool Visible = true;
    std::string Type;

    float Rotation = 0;
    int GridTile = -1;

    std::string Template;

    enum class SubTypes
    {
        None,
        Ellipse,
        Point,
        Polygon,
        Polyline,
        Text,
    };

    SubTypes SubType = SubTypes::None;

    class Property
    {
    public:
        std::string Name;
        std::string Type;
        std::string Value;
    };

    std::vector<Property> Properties;
};

class RLTilePolygonObject : public RLTileObject
{
public:
    std::vector<Vector2> Points;
};

class RLTileTextObject : public RLTileObject
{
public:
    std::string Text;
    Color TextColor = BLACK;
    bool Wrap = false;

    int FontSize = 16;
    std::string FontFamily;
    bool Bold = false;
    bool Italic = false;
    bool Underline = false;
    bool Strikeout = false;
    bool Kerning = true;
    std::string HorizontalAlignment = "left";
    std::string VerticalAlignment = "top";
};

class RLObjectLayer : public RLLayer
{
public:
    std::vector<std::shared_ptr<RLTileObject>> Objects;

    typedef std::shared_ptr<RLObjectLayer> Ptr;
};

class RLTileMap
{
public:
    std::map<int, RLTileSheet> Sheets;
    std::map<int, std::shared_ptr<RLLayer>> Layers;

    RLTiledMapTypes MapType = RLTiledMapTypes::Orthographic;

    RLTile GetTile(int x, int y, int layerID);
};

bool RLReadTileMap(const std::string& filename, RLTileMap &map);
bool RLReadTileMapFromMemory(void* buffer, size_t bufferSize, RLTileMap& map);

#endif //RLTILEMAP_H
