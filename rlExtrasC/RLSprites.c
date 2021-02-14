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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

void CopyName(const char* text, char* dest)
{
#ifdef _WIN32
    strcpy_s(dest, MAX_NAME, text);
#else
    strcpy(dest, text);
#endif
}

typedef struct RenderFrameInfo
{
    Texture* Tx;
    Rectangle* Rect;
}RenderFrameInfo;

DirectionFrameset* FindFrameset(SpriteAnimation* animation, int direction)
{
    for (size_t i = 0; i < animation->DirectionFrameCount; i++)
    {
        if (animation->DirectionFrames[i].Direction == direction)
            return &(animation->DirectionFrames[i]);
    }

    return NULL;
}

SpriteFrameCallbackFrame* FindFrameCallback(SpriteAnimation* animation, int frame)
{
    for (size_t i = 0; i < animation->FrameCallbackCount; i++)
    {
        if (animation->FrameCallbaks[i].Frame == frame)
            return &(animation->FrameCallbaks[i]);
    }

    return NULL;
}

RenderFrameInfo GetRenderFrame(Sprite* sprite, SpriteAnimation* animation, int direction, int frame)
{
    RenderFrameInfo info = { 0 };

    if (sprite != NULL)
    {
        int realFrame = 0;
        if (animation != NULL)
        {
            DirectionFrameset* frameset = FindFrameset(animation, direction);
            if (frameset == NULL)
            {
                frameset = FindFrameset(animation, DIRECTION_DEFAULT);
                if (frameset == NULL)
                    return info;
            }
            if (frame >= 0 && frame < frameset->FrameCount)
                realFrame = frameset->Frames[frame];
        }

        for (size_t i = 0; i < sprite->ImageCount; i++)
        {
            SpriteImage* image = &(sprite->Images[i]);

            size_t max = image->StartFrame + image->FrameCount;
            if (realFrame < max)
            {
                info.Tx = &(image->Sheet);
                info.Rect = &(image->Frames[realFrame - image->StartFrame]);
                return info;
            }
        }
    }

    return info;
}


SpriteAnimation CloneAnimation(SpriteAnimation* origonal)
{
    SpriteAnimation anim = { 0 };
    if (origonal != NULL)
    {
        CopyName(origonal->Name, anim.Name);
        anim.FramesPerSecond = origonal->FramesPerSecond;
        anim.Loop = origonal->Loop;
        anim.DirectionFrameCount = origonal->DirectionFrameCount;
        anim.DirectionFrames = malloc(sizeof(DirectionFrameset) * anim.DirectionFrameCount);
        for (size_t i = 0; i < anim.DirectionFrameCount; i++)
        {
            anim.DirectionFrames[i].Direction = origonal->DirectionFrames[i].Direction;
            anim.DirectionFrames[i].FrameCount = origonal->DirectionFrames[i].FrameCount;

            anim.DirectionFrames[i].Frames = malloc(sizeof(int) * anim.DirectionFrames[i].FrameCount);
            for (size_t f = 0; f < anim.DirectionFrames[i].FrameCount; f++)
            {
                anim.DirectionFrames[i].Frames[f] = origonal->DirectionFrames[i].Frames[f];
            }
        }

        anim.FrameCallbackCount = origonal->FrameCallbackCount;
        anim.FrameCallbaks = malloc(sizeof(SpriteFrameCallbackFrame) * anim.FrameCallbackCount);
        for (size_t i = 0; i < anim.FrameCallbackCount; i++)
        {
            anim.FrameCallbaks[i] = origonal->FrameCallbaks[i];
        }
    }
    return anim;
}

void ReverseAnimation(SpriteAnimation* anim)
{
    if (anim == NULL)
        return;

    size_t maxFrames = 0;

    for (size_t f = 0; f < anim->DirectionFrameCount; f++)
    {
        DirectionFrameset* frames = &(anim->DirectionFrames[f]);

        if (frames->FrameCount > maxFrames)
            maxFrames = frames->FrameCount;

        int* newFrames = malloc(sizeof(int) * frames->FrameCount);
        for (size_t i = 0; i < frames->FrameCount; i--)
            newFrames[i] = frames->Frames[frames->FrameCount - (i + 1)];
        free(frames->Frames);
        frames->Frames = newFrames;
    }

    for (size_t i = 0; i < anim->FrameCallbackCount; i++)
    {
        anim->FrameCallbaks[i].Frame = (int)maxFrames - (anim->FrameCallbaks[i].Frame + 1);
    }
}

int AddSpriteImage(Sprite* sprite, Texture tx, int xFrameCount, int yFrameCount, const char* name)
{
    if (sprite == NULL)
        return -1;

    float cellW = tx.width / (float)xFrameCount;
    float cellH = tx.height / (float)yFrameCount;

    sprite->ImageCount++;
    sprite->Images = realloc(sprite->Images, sizeof(SpriteImage) * sprite->ImageCount);

    SpriteImage* img = &(sprite->Images[sprite->ImageCount - 1]);
    img->Sheet = tx;
    CopyName(name, img->ImageSource);

    img->StartFrame = 0;
    if (sprite->ImageCount > 1)
        img->StartFrame = sprite->Images[sprite->ImageCount - 1].StartFrame + (int)sprite->Images[sprite->ImageCount - 1].FrameCount;

    img->FrameCount = xFrameCount * yFrameCount;
    img->Frames = malloc(sizeof(Rectangle) * img->FrameCount);

    size_t count = 0;
    for (float y = 0; y < tx.height; y += cellH)
    {
        for (float x = 0; x < tx.width; x += cellW)
        {
            img->Frames[count] = (Rectangle){ x, y, cellW, cellH };
            count++;
        }
    }
    return img->StartFrame;
}

int AddSpriteImageFromFile(Sprite* sprite, const char* imageName, int xFrameCount, int yFrameCount)
{
    return AddSpriteImage(sprite, LoadTexture(imageName), xFrameCount, yFrameCount, imageName);
}

void FixSpriteFrameIDs(Sprite* sprite)
{
    if (sprite == NULL)
        return;

    int count = 0;
    for (size_t i = 0; i < sprite->ImageCount; i++)
    {
        sprite->Images[i].StartFrame = count;
        count += (int)sprite->Images[i].FrameCount;
    }
}

SpriteImage* FindImageByFrame(Sprite* sprite, int frame)
{
    if (sprite != NULL)
    {
        for (size_t i = 0; i < sprite->ImageCount; i++)
        {
            int end = sprite->Images[i].StartFrame + (int)sprite->Images[i].FrameCount;
            if (frame >= sprite->Images[i].StartFrame && frame <= end)
                return &(sprite->Images[i]);
        }
    }
    return NULL;
}

int AddSpriteFlipFrames(Sprite* sprite, int startFrame, int endFrame, bool flipHorizontal, bool flipVertical)
{
    SpriteImage* img = FindImageByFrame(sprite, startFrame);
    if (img == NULL)
        return -1;

    size_t firstFlipRect = img->FrameCount;

    int localFrame = startFrame - img->StartFrame;
    int localEndFrame = endFrame - img->StartFrame;
    if (localEndFrame >= firstFlipRect)
        localEndFrame = (int)firstFlipRect - 1;

    int toAdd = endFrame - startFrame + 1;

    img->FrameCount += toAdd;
    img->Frames = realloc(img->Frames, sizeof(Rectangle) * img->FrameCount);

    float hScale = flipHorizontal ? -1.0f : 1.0f;
    float vScale = flipVertical ? -1.0f : 1.0f;

    size_t flipFrame = firstFlipRect;
    for (int i = localFrame; i <= localEndFrame; i++)
    {
        Rectangle* frameRect = &(img->Frames[i]);

        float offsetX = 0;// flipHorizontal ? frameRect.width : 0;
        float offsetY = 0;// flipVertical ? frameRect.height : 0;

        img->Frames[flipFrame] = (Rectangle){ frameRect->x + offsetX, frameRect->y + offsetY, frameRect->width * hScale, frameRect->height * vScale };
        flipFrame++;
    }

    FixSpriteFrameIDs(sprite);
    return (int)firstFlipRect;
}

SpriteAnimation* FindSpriteAnimation(Sprite* sprite, const char* name)
{
    if (sprite != NULL)
    {
        for (size_t i = 0; i < sprite->AnimationCount; i++)
        {
            if (strcmp(name, sprite->Animations[i].Name) == 0)
                return &(sprite->Animations[i]);
        }
    }

    return NULL;
}

SpriteAnimation* AddSpriteAnimationFrames(Sprite* sprite, const char* name, int direction, int start, int end)
{
    if (sprite == NULL)
        return NULL;

    SpriteAnimation* anim = FindSpriteAnimation(sprite, name);

    if (anim == NULL)
    {
        sprite->AnimationCount++;
        sprite->Animations = realloc(sprite->Animations, sizeof(SpriteAnimation) * sprite->AnimationCount);
        anim = &(sprite->Animations[sprite->AnimationCount - 1]);

        CopyName(name, anim->Name);

        anim->DirectionFrameCount = 0;
        anim->DirectionFrames = NULL;

        anim->FrameCallbackCount = 0;
        anim->FrameCallbaks = NULL;

        anim->Loop = false;
        anim->FramesPerSecond = 15;
    }

    DirectionFrameset* frameset = FindFrameset(anim, direction);
    if (frameset == NULL)
    {
        anim->DirectionFrameCount++;
        anim->DirectionFrames = realloc(anim->DirectionFrames, sizeof(DirectionFrameset) * anim->DirectionFrameCount);
        frameset = &(anim->DirectionFrames[anim->DirectionFrameCount - 1]);

        frameset->FrameCount = 0;
        frameset->Frames = NULL;
        frameset->Direction = direction;
    }

    size_t insert = frameset->FrameCount;

    if (end > start)
    {
        int count = end - start + 1;
        frameset->FrameCount += count;
        frameset->Frames = realloc(frameset->Frames, sizeof(int) * frameset->FrameCount);

        for (int i = start; i <= end; ++i)
        {
            frameset->Frames[insert] = i;
            insert++;
        }

    }
    else
    {
        int count = start - end + 1;
        frameset->FrameCount += count;
        frameset->Frames = realloc(frameset->Frames, sizeof(int) * frameset->FrameCount);

        for (int i = start; i >= end; --i)
        {
            frameset->Frames[insert] = i;
            insert++;
        }
    }

    return anim;
}

void SetSpriteAnimationLoop(Sprite* sprite, const char* name, bool loop)
{
    if (sprite != NULL)
    {
        SpriteAnimation* anim = FindSpriteAnimation(sprite, name);
        if (anim != NULL)
            anim->Loop = loop;
    }
}

void SetSpriteAnimationSpeed(Sprite* sprite, const char* name, float fps)
{
    if (sprite != NULL)
    {
        SpriteAnimation* anim = FindSpriteAnimation(sprite, name);
        if (anim != NULL)
            anim->FramesPerSecond = fps;
    }
}

void AddSpriteAnimationFrameCallback(Sprite* sprite, const char* animationName, const char* frameName, int frame, SpriteFrameCallback callback)
{
    if (sprite != NULL)
    {
        SpriteAnimation* anim = FindSpriteAnimation(sprite, animationName);
        if (anim != NULL)
        {
            anim->FrameCallbackCount++;
            anim->FrameCallbaks = realloc(anim->FrameCallbaks, sizeof(SpriteFrameCallbackFrame) * anim->FrameCallbackCount);
            anim->FrameCallbaks[anim->FrameCallbackCount - 1].Callback = callback;
            memset(anim->FrameCallbaks[anim->FrameCallbackCount - 1].Name, 0, MAX_NAME);
            CopyName(frameName, anim->FrameCallbaks[anim->FrameCallbackCount - 1].Name);
            anim->FrameCallbaks[anim->FrameCallbackCount - 1].Frame = frame;
        }
    }
}


void SetSpriteAnimationFrameCallback(Sprite* sprite, const char* animationName, const char* frameName, SpriteFrameCallback callback)
{
    if (sprite != NULL)
    {
        SpriteAnimation* anim = FindSpriteAnimation(sprite, animationName);
        if (anim != NULL)
        {
            for (size_t i = 0; i < anim->FrameCallbackCount; i++)
            {
                if (strcmp(frameName, anim->FrameCallbaks[i].Name) == 0)
                {
                    anim->FrameCallbaks[i].Callback = callback;
                    return;
                }
            }
        }
    }
}


void SaveSpriteInfo(Sprite* sprite, const char* filePath)
{
    if (sprite == NULL)
        return;

    FILE* fp = fopen(filePath, "w");
    if (fp == NULL)
        return;

    fprintf(fp, "RLSprite V:1\nImages %zu\n", sprite->ImageCount);
    for (size_t i = 0; i < sprite->ImageCount; i++)
    {
        fprintf(fp,"Image %s\n", sprite->Images[i].ImageSource);
        fprintf(fp, "Frameset %d %zu\n", sprite->Images[i].StartFrame,sprite->Images[i].FrameCount);
        for (int f = 0; f < sprite->Images[i].FrameCount; f++)
            fprintf(fp, "%f %f %f %f\n", sprite->Images[i].Frames[f].x, sprite->Images[i].Frames[f].y, sprite->Images[i].Frames[f].width, sprite->Images[i].Frames[f].height);
    }

    fprintf(fp, "Animations %zu\n", sprite->AnimationCount);
    for (size_t i = 0; i < sprite->AnimationCount; i++)
    {
        fprintf(fp, "Animation %s\n", sprite->Animations[i].Name);
        fprintf(fp, "Options %f %s\n", sprite->Animations[i].FramesPerSecond, sprite->Animations[i].Loop ? "loop" : "once");
        fprintf(fp, "Framesets %zu\n", sprite->Animations[i].DirectionFrameCount);

        for (size_t d = 0; d < sprite->Animations[i].DirectionFrameCount; d++)
        {
            fprintf(fp, "Frames %zu %d\n", sprite->Animations[i].DirectionFrames[d].FrameCount, sprite->Animations[i].DirectionFrames[d].Direction);
            for (size_t f = 0; f < sprite->Animations[i].DirectionFrames[d].FrameCount; f++)
                fprintf(fp, " %d", sprite->Animations[i].DirectionFrames[d].Frames[f]);
            fprintf(fp, "\n");
        }

        fprintf(fp, "NamedFrames %zu\n", sprite->Animations[i].FrameCallbackCount);
        for (size_t c = 0; c < sprite->Animations[i].FrameCallbackCount;c++)
        {
            fprintf(fp, "%d %s\n", sprite->Animations[i].FrameCallbaks[c].Frame, sprite->Animations[i].FrameCallbaks[c].Name);
        }
    }

    fclose(fp);
}

bool LoadSpriteInfo(Sprite* sprite, const char* filePath)
{
    if (sprite == NULL)
        return false;

    FILE* fp = fopen(filePath, "r");
    if (fp == NULL)
        return false;

    bool bad = false;
    int version = 0;

    if (fscanf(fp, "RLSprite V:%d\n", &version) == 1 && version == 1)
    {
        if (fscanf(fp, "Images %zu\n", &(sprite->ImageCount)) == 1)
        {
            sprite->Images = malloc(sizeof(SpriteImage) * sprite->ImageCount);
            for (size_t i = 0; i < sprite->ImageCount; i++)
            {
                SpriteImage* image = &(sprite->Images[i]);
                memset(image->ImageSource, 0, MAX_NAME);
              
                if (fscanf(fp, "Image %s\n", image->ImageSource) == 1 && fscanf(fp, "Frameset %d %zu\n", &image->StartFrame, &image->FrameCount) == 2)
                {
                    image->Sheet = LoadTexture(image->ImageSource);

                    image->Frames = malloc(sizeof(Rectangle) * image->FrameCount);

                    for (int f = 0; f < image->FrameCount; f++)
                        fscanf(fp, "%f %f %f %f\n", &sprite->Images[i].Frames[f].x, &sprite->Images[i].Frames[f].y, &sprite->Images[i].Frames[f].width, &sprite->Images[i].Frames[f].height);
                }
                else
                {
                    bad = true;
                    break;
                }
            }

            if (fscanf(fp, "Animations %zu\n", &sprite->AnimationCount) == 1)
            {
                sprite->Animations = malloc(sizeof(SpriteAnimation) * sprite->AnimationCount);
                for (size_t i = 0; i < sprite->AnimationCount; i++)
                {
                    SpriteAnimation* animation = &(sprite->Animations[i]);
                    memset(animation->Name, 0, MAX_NAME);
                    animation->FrameCallbackCount = 0;
                    animation->FrameCallbaks = NULL;

                    char temp[128] = { 0 };
                    if (fscanf(fp, "Animation %s\n", animation->Name) == 1 && fscanf(fp, "Options %f %s\n", &animation->FramesPerSecond, temp) == 2 && fscanf(fp, "Framesets %zu\n", &animation->DirectionFrameCount) == 1)
                    {
                        animation->Loop = temp[0] == 'l';
                        animation->DirectionFrames = malloc(sizeof(DirectionFrameset) * animation->DirectionFrameCount);

                        for (size_t d = 0; d < animation->DirectionFrameCount; d++)
                        {
                            DirectionFrameset* frameset = &(animation->DirectionFrames[d]);

                            if (fscanf(fp, "Frames %zu %d\n", &frameset->FrameCount, &frameset->Direction) == 2)
                            {
                                frameset->Frames = malloc(sizeof(int) * frameset->FrameCount);
                                for (size_t f = 0; f < frameset->FrameCount; f++)
                                {
                                    if (fscanf(fp, " %d", &frameset->Frames[f]) != 1)
                                    {
                                        bad = true;
                                        break;
                                    }
                                }
                                fscanf(fp, "\n");
                            }
                            else
                                bad = true;
                        }

                        if (fscanf(fp, "NamedFrames %zu\n", &animation->FrameCallbackCount) == 1)
                        {
                            if (animation->FrameCallbackCount > 0)
                            {
                                animation->FrameCallbaks = malloc(sizeof(SpriteFrameCallbackFrame) * animation->FrameCallbackCount);
                                for (size_t c = 0; c < animation->FrameCallbackCount; c++)
                                {
                                    animation->FrameCallbaks[c].Callback = NULL;
                                    memset(animation->FrameCallbaks[c].Name, 0, MAX_NAME);
                                    if (fscanf(fp, "%d %s\n", &animation->FrameCallbaks[c].Frame, animation->FrameCallbaks[c].Name) != 2)
                                    {
                                        bad = true;
                                        break;
                                    }
                                }
                            }
                        }
                        else
                            bad = true;
                    }
                    else
                        bad = true;
                }
            }
            else
                bad = true;
        }
        else
            bad = true;
    }
    else
        bad = true;

    fclose(fp);

    return !bad;
}

void FreeSprite(Sprite* sprite)
{
    if (sprite == NULL)
        return;

    for (size_t i = 0; i < sprite->ImageCount; i++)
    {
        free(sprite->Images[i].Frames);
    }
    free(sprite->Images);
    sprite->ImageCount = 0;

    for (size_t i = 0; i < sprite->AnimationCount; i++)
    {
        for (size_t d = 0; d < sprite->Animations[i].DirectionFrameCount; d++)
        {
            free(sprite->Animations[i].DirectionFrames[d].Frames);
        }
        free(sprite->Animations[i].DirectionFrames);

        free(sprite->Animations[i].FrameCallbaks);
    }

    free(sprite->Animations);
    sprite->AnimationCount = 0;
}

SpriteInstance* CreateSpriteInstance(Sprite* sprite, Color tint)
{
    SpriteInstance* instance = malloc(sizeof(SpriteInstance));
    if (instance == NULL)
        return NULL;

    instance->NameFrameTrigger = NULL;
    instance->Speed = 1;
    instance->Scale = 1;
    instance->Rotation = 0;
    instance->OriginX = OriginMinium;
    instance->OriginY = OriginMinium;

    instance->LayerCount = 0;
    instance->Layers = NULL;

    instance->Position = (Vector2){ 0,0 };
    instance->Direction = DIRECTION_DEFAULT;
    instance->LastRectangle = (Rectangle){ 0,0,0,0 };

    instance->CurrentAnimation = NULL;

    instance->CurrentFrame = 0;
    instance->CurrentDirection = 0;
    instance->CurrentRealFrame = 0;
    instance->LastFrameTime =  -99999999;

    if (sprite != NULL)
    {
        AddSpriteInstancLayer(instance, sprite, tint);
    }

    return instance;
}

int AddSpriteInstancLayer(SpriteInstance* instance, Sprite* sprite, Color tint)
{
    if (instance == NULL)
        return -1;

    instance->LayerCount++;
    instance->Layers = realloc(instance->Layers, sizeof(SpriteInstanceLayer) * instance->LayerCount);
    instance->Layers[instance->LayerCount - 1].Image = sprite;
    instance->Layers[instance->LayerCount - 1].Tint = tint;

    return (int)(instance->LayerCount - 1);
}

void SetSpriteInstanceAimation(SpriteInstance* instance, const char* name)
{
    if (instance == NULL || instance->LayerCount == 0 || (instance->CurrentAnimation != NULL && strcmp(instance->CurrentAnimation->Name, name) == 0))
        return;

    instance->CurrentAnimation = FindSpriteAnimation(instance->Layers[0].Image, name);
    instance->CurrentFrame = 0;
    instance->LastFrameTime = GetTime();
}

void UpdateSpriteInstance(SpriteInstance* instance)
{
    if (instance == NULL || instance->CurrentAnimation == NULL)
        return;

    instance->NameFrameTrigger = NULL;

    double now = GetTime();
    double frameTime = 1.0 / ((double)instance->CurrentAnimation->FramesPerSecond * instance->Speed);

    if (instance->LastFrameTime <= 0)
        instance->LastFrameTime = now;

    instance->CurrentDirection = instance->Direction;

    DirectionFrameset* dirFrames = FindFrameset(instance->CurrentAnimation, instance->CurrentDirection);

    if (dirFrames == NULL)
    {
        instance->CurrentDirection = DIRECTION_DEFAULT;
        dirFrames = FindFrameset(instance->CurrentAnimation, instance->CurrentDirection);

        if (dirFrames == NULL)
            return;
    }

    if (dirFrames->FrameCount <= 1)
    {
        instance->CurrentFrame = 0;
        return;
    }

    double frameChangeTime = instance->LastFrameTime + frameTime;

    if (frameChangeTime <= now)
    {
        instance->CurrentFrame++;

        SpriteFrameCallbackFrame* callback = FindFrameCallback(instance->CurrentAnimation, instance->CurrentFrame);

        if (callback != NULL)
        {
            instance->NameFrameTrigger = callback->Name;
            if (callback->Callback != NULL)
                callback->Callback(instance, instance->CurrentFrame);
        }
            
        instance->LastFrameTime = now;
        if (instance->CurrentFrame >= dirFrames->FrameCount)
        {
            if (instance->CurrentAnimation->Loop)
            {
                instance->CurrentFrame = 0;
            }
            else
            {
                instance->CurrentFrame--;
                instance->LastFrameTime = 99999999999;

                callback = FindFrameCallback(instance->CurrentAnimation, -1);
                if (callback != NULL)
                {
                    instance->NameFrameTrigger = callback->Name;
                    if (callback->Callback != NULL)
                        callback->Callback(instance, -1);
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
    case OriginMinium:
        return 0;
    case OriginCenter:
        return max * 0.5f;
    case OriginMaximum:
        return max;
    }
}

void RenderSpriteInstance(SpriteInstance* instance)
{
    if (instance == NULL)
        return;

    for (size_t i = 0; i < instance->LayerCount; i++)
    {
        RenderFrameInfo frame = GetRenderFrame(instance->Layers[i].Image, instance->CurrentAnimation, instance->CurrentDirection, instance->CurrentFrame);
        if (frame.Rect != NULL)
            instance->LastRectangle = *frame.Rect;

        Rectangle dest = { instance->Position.x, instance->Position.y, (float)fabs(instance->LastRectangle.width) * instance->Scale, (float)fabs(instance->LastRectangle.height) * instance->Scale };
        Vector2  center = { GetOriginValue(instance->OriginX,dest.width), GetOriginValue(instance->OriginY, dest.height) };

        DrawTexturePro(*frame.Tx, instance->LastRectangle, dest, center, instance->Rotation, instance->Layers[i].Tint);
    }
}

void UpdateRenderSpriteInstance(SpriteInstance* instance)
{
    UpdateSpriteInstance(instance);
    RenderSpriteInstance(instance);
}

bool SpriteInstanceFrameTriggered(SpriteInstance* instance, const char* name)
{
    return instance != NULL && instance->NameFrameTrigger != NULL && strcmp(instance->NameFrameTrigger, name) == 0;
}

void FreeSpriteInstance(SpriteInstance* instance)
{
    if (instance == NULL)
        return;

    free(instance->Layers);
    instance->Layers = NULL;
    instance->LayerCount = 0;

    instance->CurrentAnimation = NULL;

    free(instance);
}