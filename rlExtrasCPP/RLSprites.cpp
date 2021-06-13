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

#include "RLSprites.h"
#include <stdio.h>
#include <algorithm>

namespace RLSprites
{
    std::pair<Texture*, Rectangle*> GetRenderFrame(Sprite* sprite, SpriteAnimation* animation, int direction, int frame)
    {
        if (sprite != nullptr)
        {
            int realFrame = 0;
            if (animation != nullptr)
            {
                std::vector<int>& directionFrames = animation->DirectionFrames[direction];
                realFrame = directionFrames[frame];
            }

            size_t count = 0;
            for (size_t i = 0; i < sprite->Images.size(); ++i)
            {
                SpriteImage& image = sprite->Images[i];

                size_t max = image.Frames.size() + count;
                if (realFrame < max)
                {
                    return std::pair<Texture*, Rectangle*>(&image.Sheet, &(image.Frames[realFrame - count]));
                }

                count += image.Frames.size();
            }
        }

        return std::pair<Texture*, Rectangle*>(nullptr, nullptr);
    }


    SpriteAnimation SpriteAnimation::Clone()
    {
        SpriteAnimation anim;
        anim.Name = Name;
        anim.FramesPerSecond = FramesPerSecond;
        anim.Loop = Loop;
        anim.DirectionFrames = DirectionFrames;
        anim.FrameCallbacks = FrameCallbacks;

        return anim;
    }

    void SpriteAnimation::Reverse()
    {
        int max = 0;
        for (std::map<int, std::vector<int>>::iterator itr = DirectionFrames.begin(); itr != DirectionFrames.end(); itr++)
        {
            std::reverse(itr->second.begin(), itr->second.end());
            if (itr->second.size() > max)
                max = (int)itr->second.size();
        }

        std::map<int, SpriteFrameInfo> newCallbacks;
        for (auto& cb : FrameCallbacks)
        {
            newCallbacks[(max - 1) - cb.first] = cb.second;
        }

        FrameCallbacks = newCallbacks;
    }

    int Sprite::AddImage(Texture tx, int xFrameCount, int yFrameCount, const char* name)
    {
        float cellW = tx.width / (float)xFrameCount;
        float cellH = tx.height / (float)yFrameCount;

        SpriteImage img;
        img.Sheet = tx;
        img.ImageSource = name;

        img.StartFrame = 0;
        if (!Images.empty())
            img.StartFrame = Images[(int)Images.size() - 1].StartFrame + (int)Images[(int)Images.size() - 1].Frames.size();

        for (float y = 0; y < tx.height; y += cellH)
        {
            for (float x = 0; x < tx.width; x += cellW)
            {
                Rectangle rect = { x, y, cellW, cellH };
                img.Frames.emplace_back(rect);
            }
        }

        Images.emplace_back(img);
        return img.StartFrame;
    }

    int Sprite::AddImage(const std::string& imageName, int xFrameCount, int yFrameCount)
    {
        Texture tx = LoadTexture(imageName.c_str());

        return AddImage(tx, xFrameCount, yFrameCount, imageName.c_str());
    }

    void FixSpriteFrameIDs(Sprite* sprite)
    {
        int count = 0;
        for (auto& img : sprite->Images)
        {
            img.StartFrame = count;
            count += (int)img.Frames.size();
        }
    }

    SpriteImage* FindImageByFrame(Sprite* sprite, int frame)
    {
        if (sprite == nullptr)
            return nullptr;

        for (std::vector<SpriteImage>::iterator itr = sprite->Images.begin(); itr != sprite->Images.end(); itr++)
        {
            int end = itr->StartFrame + (int)itr->Frames.size();

            if (frame >= itr->StartFrame && frame <= end)
                return &(*itr);
        }

        return nullptr;
    }

    int Sprite::AddFlipFrames(int startFrame, int endFrame, bool flipHorizontal, bool flipVertical)
    {
        SpriteImage* img = FindImageByFrame(this, startFrame);
        if (img == nullptr)
            return -1;

        std::vector<Rectangle> flipRects;
        int localFrame = startFrame - img->StartFrame;

        float hScale = flipHorizontal ? -1.0f : 1.0f;
        float vScale = flipVertical ? -1.0f : 1.0f;

        for (int i = localFrame; i < img->Frames.size() && i <= endFrame - startFrame; ++i)
        {
            Rectangle& frameRect = img->Frames[i];

            float offsetX = 0;// flipHorizontal ? frameRect.width : 0;
            float offsetY = 0;// flipVertical ? frameRect.height : 0;

            flipRects.emplace_back(Rectangle{ frameRect.x + offsetX, frameRect.y + offsetY, frameRect.width * hScale, frameRect.height * vScale });
        }

        int newStart = img->StartFrame + (int)img->Frames.size();

        for (auto& newRect : flipRects)
            img->Frames.emplace_back(newRect);

        FixSpriteFrameIDs(this);
        return newStart;
    }

    SpriteAnimation* Sprite::FindAnimation(const std::string& name)
    {
        std::map<std::string, SpriteAnimation>::iterator itr = Animations.find(name);
        if (itr == Animations.end())
            return nullptr;

        return &(itr->second);
    }

    void Sprite::AddAnimation(SpriteAnimation& animation)
    {
        Animations[animation.Name] = animation;
    }

    SpriteAnimation* Sprite::AddAnimation(const std::string name, int direction, int start, int end)
    {
        SpriteAnimation* anim = FindAnimation(name);
        if (anim == nullptr)
        {
            AddAnimation(SpriteAnimation() = { name });
            anim = FindAnimation(name);
        }

        std::vector<int> frames;
        if (end > start)
        {
            for (int i = start; i <= end; ++i)
                frames.push_back(i);
        }
        else
        {
            for (int i = start; i >= end; --i)
                frames.push_back(i);
        }
        anim->DirectionFrames[direction] = frames;

        return anim;
    }

    void Sprite::SetAnimationLoop(const std::string& name, bool loop)
    {
        SpriteAnimation* anim = FindAnimation(name);
        if (anim == nullptr)
            return;

        anim->Loop = loop;
    }

    void Sprite::SetAnimationSpeed(const std::string& name, float fps)
    {
        SpriteAnimation* anim = FindAnimation(name);
        if (anim == nullptr)
            return;

        anim->FramesPerSecond = fps;
    }

    void Sprite::AddAnimationFrameCallback(const std::string& animationName, SpriteFrameCallback callback, const std::string& frameName, int frame)
    {
        SpriteAnimation* anim = FindAnimation(animationName);
        if (anim == nullptr)
            return;

        SpriteFrameInfo info;
        info.Name = frameName;
        info.Callback = callback;

        anim->FrameCallbacks[frame] = info;
    }

    void Sprite::SetAnimationFrameCallback(const std::string& animationName, SpriteFrameCallback callback, const std::string& frameName)
    {
        SpriteAnimation* anim = FindAnimation(animationName);
        if (anim == nullptr)
            return;

        for (auto& cb : anim->FrameCallbacks)
        {
            if (cb.second.Name == frameName)
            {
                cb.second.Callback = callback;
                return;
            }
        }
    }

    bool Sprite::Save(const char* filePath)
    {
        FILE* fp = fopen(filePath, "w");
        if (fp == NULL)
            return false;

        fprintf(fp, "RLSprite V:1\nImages %zu\n", Images.size());
        for (auto& image : Images)
        {
            fprintf(fp, "Image %s\n", image.ImageSource.c_str());
            fprintf(fp, "Frameset %d %zu\n", image.StartFrame, image.Frames.size());
            for (auto& frame : image.Frames)
                fprintf(fp, "%f %f %f %f\n", frame.x, frame.y, frame.width, frame.height);
        }

        fprintf(fp, "Animations %zu\n", Animations.size());
        for (auto animation : Animations)
        {
            fprintf(fp, "Animation %s\n", animation.first.c_str());
            fprintf(fp, "Options %f %s\n", animation.second.FramesPerSecond, animation.second.Loop ? "loop" : "once");
            fprintf(fp, "Framesets %zu\n", animation.second.DirectionFrames.size());

            for (auto frame : animation.second.DirectionFrames)
            {
                fprintf(fp, "Frames %zu %d\n", frame.second.size(), frame.first);
                for (int i : frame.second)
                    fprintf(fp, " %d", i);
                fprintf(fp, "\n");
            }

            fprintf(fp, "NamedFrames %zu\n", animation.second.FrameCallbacks.size());
            for (auto a : animation.second.FrameCallbacks)
            {
                fprintf(fp, "%d %s\n", a.first, a.second.Name.c_str());
            }
        }

        fclose(fp);

        return true;
    }

    Sprite Sprite::Load(const char* filePath)
    {
        Sprite sprite;

        FILE* fp = fopen(filePath, "r");
        if (fp == nullptr)
            return sprite;

        int version = 0;

        char tempStr[512];
        size_t tempSize = 0, tempSize2 = 0, tempSize3;

        if (fscanf(fp, "RLSprite V:%d\n", &version) == 1 && version == 1)
        {
            size_t imageCount = 0;
            if (fscanf(fp, "Images %zu\n", &(imageCount)) == 1)
            {
                for (size_t i = 0; i < imageCount; i++)
                {
                    SpriteImage image;

                    if (fscanf(fp, "Image %s\n", tempStr) == 1 && fscanf(fp, "Frameset %d %zu\n", &image.StartFrame, &tempSize) == 2)
                    {
                        image.ImageSource = tempStr;
                        image.Sheet = LoadTexture(image.ImageSource.c_str());

                        for (int f = 0; f < tempSize; f++)
                        {
                            Rectangle rect = { 0,0,0,0 };
                            fscanf(fp, "%f %f %f %f\n", &rect.x, &rect.y, &rect.width, &rect.height);
                            image.Frames.push_back(rect);
                        }
                        sprite.Images.emplace_back(image);
                    }
                }

                if (fscanf(fp, "Animations %zu\n", &tempSize) == 1)
                {
                    for (size_t i = 0; i < tempSize; i++)
                    {
                        SpriteAnimation animation;

                        char temp[128] = { 0 };
                        if (fscanf(fp, "Animation %s\n", tempStr) == 1 && fscanf(fp, "Options %f %s\n", &animation.FramesPerSecond, temp) == 2 && fscanf(fp, "Framesets %zu\n", &tempSize2) == 1)
                        {
                            animation.Name = tempStr;
                            animation.Loop = temp[0] == 'l';

                            for (size_t d = 0; d < tempSize2; d++)
                            {
                                int direction = 0;
                                if (fscanf(fp, "Frames %zu %d\n", &tempSize3, &direction) == 2)
                                {
                                    animation.DirectionFrames[direction] = std::vector<int>();

                                    for (size_t f = 0; f < tempSize3; f++)
                                    {
                                        int frame = 0;
                                        if (fscanf(fp, " %d", &frame) == 1)
                                        {
                                            animation.DirectionFrames[direction].push_back(frame);
                                        }
                                    }
                                    fscanf(fp, "\n");
                                }
                            }

                            if (fscanf(fp, "NamedFrames %zu\n", &tempSize2) == 1)
                            {
                                if (tempSize2 > 0)
                                {

                                    for (size_t c = 0; c < tempSize2; c++)
                                    {
                                        SpriteFrameInfo frameCallbackInfo;
                                        int frame = 0;
                                        if (fscanf(fp, "%d %s\n", &frame, tempStr) == 2)
                                        {
                                            frameCallbackInfo.Name = tempStr;
                                            animation.FrameCallbacks[frame] = frameCallbackInfo;
                                        }
                                    }
                                }
                            }

                            sprite.Animations[animation.Name] = animation;
                        }
                    }
                }
            }
        }

        fclose(fp);

        return sprite;
    }

    void SpriteInstance::SetAimation(const std::string& name)
    {
        if (Layers.empty() || (CurrentAnimation != nullptr && CurrentAnimation->Name == name))
            return;

        CurrentAnimation = Layers[0].Image->FindAnimation(name);
        CurrentFrame = 0;
        LastFrameTime = GetTime();
    }

    void SpriteInstance::Update()
    {
        TriggerFrameName.clear();

        if (CurrentAnimation == nullptr)
            return;

        double now = GetTime();
        double frameTime = 1.0 / ((double)CurrentAnimation->FramesPerSecond * Speed);

        if (LastFrameTime <= 0)
            LastFrameTime = now;

        CurrentDirection = Direction;
        if (CurrentAnimation->DirectionFrames.find(CurrentDirection) == CurrentAnimation->DirectionFrames.end())
        {
            CurrentDirection = DIRECTION_DEFAULT;
            if (CurrentAnimation->DirectionFrames.find(CurrentDirection) == CurrentAnimation->DirectionFrames.end())
                return;
        }

        if (CurrentAnimation->DirectionFrames[CurrentDirection].size() <= 1)
        {
            CurrentFrame = 0;
            return;
        }

        double frameChangeTime = LastFrameTime + frameTime;

        if (frameChangeTime <= now)
        {
            ++CurrentFrame;

            auto itr = CurrentAnimation->FrameCallbacks.find(CurrentFrame);
            if (itr != CurrentAnimation->FrameCallbacks.end())
            {
                TriggerFrameName = itr->second.Name;
                if (itr->second.Callback != nullptr)
                    itr->second.Callback(this, CurrentFrame);
            }

            LastFrameTime = now;
            if (CurrentFrame >= CurrentAnimation->DirectionFrames[CurrentDirection].size())
            {
                if (CurrentAnimation->Loop)
                {
                    CurrentFrame = 0;
                }
                else
                {
                    --CurrentFrame;
                    LastFrameTime = 99999999999;

                    auto itr = CurrentAnimation->FrameCallbacks.find(-1);
                    if (itr != CurrentAnimation->FrameCallbacks.end())
                    {
                        TriggerFrameName = itr->second.Name;
                        if (itr->second.Callback != nullptr)
                            itr->second.Callback(this, -1);
                    }
                }
            }
        }
    }

    float GetOriginValue(OriginLocations origin, float max)
    {
        switch (origin)
        {
        default:
        case OriginLocations::Minium:
            return 0;
        case OriginLocations::Center:
            return max * 0.5f;
        case OriginLocations::Maximum:
            return max;
        }
    }

    void SpriteInstance::Render()
    {
        for (auto& sprite : Layers)
        {
            auto frame = GetRenderFrame(sprite.Image, CurrentAnimation, CurrentDirection, CurrentFrame);
            if (frame.second != nullptr)
                LastRectangle = *frame.second;

            Rectangle dest = { Position.x,Position.y,(float)fabs(LastRectangle.width) * Scale, (float)fabs(LastRectangle.height) * Scale };
            Vector2  center = { GetOriginValue(OriginX,dest.width), GetOriginValue(OriginY, dest.height) };

            DrawTexturePro(*frame.first, LastRectangle, dest, center, Rotation, sprite.Tint);
        }
    }

    void SpriteInstance::UpdateRender()
    {
        Update();
        Render();
    }
}