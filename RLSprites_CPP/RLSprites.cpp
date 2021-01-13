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
        std::string Name;
        float FramesPerSecond = 15;
        bool Loop = false;
        std::map<int, std::vector<int>> DirectionFrames;

        std::map<int, SpriteFrameCallback> FrameCallbaks;


		SpriteAnimation anim;
		anim.Name = Name;
		anim.FramesPerSecond = FramesPerSecond;
		anim.Loop = Loop;
		anim.DirectionFrames = DirectionFrames;
		anim.FrameCallbaks = FrameCallbaks;

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

		std::map<int, SpriteFrameCallback> newCallbacks;
		for (auto& cb : FrameCallbaks)
		{
			newCallbacks[(max - 1) - cb.first] = cb.second;
		}
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
		auto& itr = Animations.find(name);
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

	void Sprite::AddAnimationFrameCallback(const std::string& name, SpriteFrameCallback callback, int frame)
	{
        SpriteAnimation* anim = FindAnimation(name);
        if (anim == nullptr)
            return;

		anim->FrameCallbaks[frame] = callback;
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

			auto itr = CurrentAnimation->FrameCallbaks.find(CurrentFrame);
			if (itr != CurrentAnimation->FrameCallbaks.end())
				itr->second(this, CurrentFrame);

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

					auto itr = CurrentAnimation->FrameCallbaks.find(-1);
					if (itr != CurrentAnimation->FrameCallbaks.end())
						itr->second(this, -1);
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

			Rectangle dest = { Postion.x,Postion.y,fabs(LastRectangle.width) * Scale, fabs(LastRectangle.height) * Scale };
			Vector2  center = { GetOriginValue(OriginX,dest.width), GetOriginValue(OriginY, dest.height) };

			DrawTexturePro(*frame.first, *frame.second, dest, center, Rotation, sprite.Tint);
		}
	}

	void SpriteInstance::UpdateRender()
	{
		Update();
		Render();
	}
}