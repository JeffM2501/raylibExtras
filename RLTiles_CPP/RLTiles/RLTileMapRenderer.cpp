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

#include "RLTileMapRenderer.h"

Vector2 GetDisplayLocation(int x, int y, int TileWidth, int TileHeight, RLTiledMapTypes mapType)
{
    if (mapType == RLTiledMapTypes::Orthographic)
        return Vector2{ static_cast<float>(x * TileWidth), static_cast<float>(y * TileHeight) };

    float halfWidth = TileWidth * 0.5f;
    float halfHeight = TileHeight * 0.5f;
    float quarterHeight = TileHeight * 0.25f;

    return Vector2{ x * halfWidth - y * halfWidth - halfWidth, y * halfHeight + (x * halfHeight) };
}

void RLTileRenderer::Setup()
{
    TextureCache.clear();

    for (std::map<int, RLTileSheet>::iterator sheet = TileMap.Sheets.begin(); sheet != TileMap.Sheets.end(); sheet++)
    {
        int start = sheet->second.StartFrame;
        int end = ((int)sheet->second.Tiles.size() - 1) + start;

        TextureCache[std::pair<int,int>(start,end)] = std::pair<Texture, RLTileSheet*>(LoadTexture(sheet->second.SheetSource.c_str()), (&sheet->second));
    }

    RenderLayers.clear();
    for (auto layer : TileMap.Layers)
    {
        RenderLayers.emplace(layer.first, RLTileRenderLayer());

        RLTileRenderLayer& renderLayer = RenderLayers[layer.first];

        renderLayer.TileSize.x = layer.second.TileWidth;
        renderLayer.TileSize.y = layer.second.TileHeight;

        for (int y = 0; y < layer.second.Height; ++y)
        {
            for (int x = 0; x < layer.second.Width; ++x)
            {
                RLTile tile = TileMap.GetTile(x, y, layer.first);

                auto itr = std::find_if(TextureCache.begin(), TextureCache.end(), [tile](const auto tx)
                    {
                        return (tile.TileID >= tx.first.first && tile.TileID <= tx.first.second);
                    });

                RLRenderTile tileInfo;
                if (itr != TextureCache.end())
                {
                    tileInfo.SourceTexture = &(itr->second.first);
                    tileInfo.SourceRect = itr->second.second->GetFrame(tile.TileID);
                    if (tileInfo.SourceRect.width == 0 || tileInfo.SourceRect.height == 0)
                        tileInfo.SourceRect = Rectangle{ 0,0, (float)tileInfo.SourceTexture->width, (float)tileInfo.SourceTexture->height };

                    if (tile.FlipDiag)
                        tileInfo.Rotate = true;
                    if (tile.FlipX)
                        tileInfo.SourceRect.width *= -1;
                    if (tile.FilpY)
                        tileInfo.SourceRect.height *= -1;

                    tileInfo.DestinationPos = GetDisplayLocation(x, y, layer.second.TileWidth, layer.second.TileHeight, TileMap.MapType);
                }
                renderLayer.RenderTiles.emplace_back(tileInfo);
            }
        }
    }
}

bool RLTileRenderer::TileInView(RLRenderTile& tile)
{
    if (tile.DestinationPos.x + tile.SourceRect.width < CurrentViewRect.x)
        return false;

    if (tile.DestinationPos.y + tile.SourceRect.height < CurrentViewRect.y)
        return false;

    if (tile.DestinationPos.x > CurrentViewRect.x + CurrentViewRect.width)
        return false;


    if (tile.DestinationPos.y > CurrentViewRect.y + CurrentViewRect.height)
        return false;

    return true;
}

void RLTileRenderer::Draw(Camera2D& camera)
{
    CurrentViewRect.x = camera.target.x - (camera.offset.x / camera.zoom);
    CurrentViewRect.y = camera.target.y - (camera.offset.y / camera.zoom);

    CurrentViewRect.width = GetScreenWidth() / camera.zoom;
    CurrentViewRect.height = GetScreenHeight() / camera.zoom;

    for (auto layer : RenderLayers)
    {
        for (auto& tile : layer.second.RenderTiles)
        {
            if (tile.SourceTexture != nullptr && TileInView(tile))
            {
                Vector2 origin = { layer.second.TileSize.x * 0.5f, layer.second.TileSize.y * 0.5f };
                Rectangle dest = { tile.DestinationPos.x + origin.x,tile.DestinationPos.y + origin.y, layer.second.TileSize.x, layer.second.TileSize.y };
               
                DrawTexturePro(*tile.SourceTexture, tile.SourceRect, dest, origin, tile.Rotate ? 90 : 0, WHITE);       // Draw a part of a texture defined by a rectangle with 'pro' parameters
            }
        }   
    }
}
