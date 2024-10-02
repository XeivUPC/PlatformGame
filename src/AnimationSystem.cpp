#include "AnimationSystem.h"
#include "Timer.h"
#include "Render.h"
#include "Engine.h"

SDL_Rect Animation::GetSpriteRect(int spriteIndex)
{
	SDL_Rect spriteRect = SDL_Rect();
	spriteRect.x = sprites[spriteIndex].index.getX();
	spriteRect.y = sprites[spriteIndex].index.getY();
	spriteRect.w = sprites[spriteIndex].size.getX();
	spriteRect.h = sprites[spriteIndex].size.getY();
	return spriteRect;
}

SDL_Texture* Animation::GetTexture(int spriteIndex)
{
	return sprites[spriteIndex].texture;
}

int Animation::GetCount(int spriteIndex)
{
	return sprites.size();
}

Animation::Animation(const char* n, std::vector<Sprite> s)
{
	name = n;
	sprites = s;
}

void Animator::Next()
{
	if (customSpriteRangeStart != -1 && customSpriteRangeEnd != -1)
	{
		if (currentSprite < animations.size()-1)
			currentSprite++;
		else
		{
			if (loop)
			{
				currentSprite = 0;
			}
			else
			{
				currentSprite = animations.size() - 1;
			}
		}
	}
	else
	{
		if (currentSprite < customSpriteRangeEnd)
			currentSprite++;
		else
		{
			if (loop)
			{
				currentSprite = customSpriteRangeStart;
			}
			else
			{
				currentSprite = customSpriteRangeEnd;
			}
		}
	}
}

Animator::Animator(std::vector<Animation> anims, float s)
{
	for (int i = 0; i < anims.size(); i++)
	{
		animations[anims[i].name] = anims[i];
	}
	speed = s;
}

void Animator::SelectAnimation(const char* animName, bool l)
{
	currentAnimation = animName;
	loop = l;
	customSpriteRangeStart = -1;
	customSpriteRangeEnd = -1;
}

void Animator::SelectAnimation(const char* animName, bool l, int indexInit, int indexEnd)
{
	currentAnimation = animName;
	loop = l;
	customSpriteRangeStart = indexInit;
	customSpriteRangeEnd = indexEnd;
}

void Animator::SetSpeed(float s)
{
	speed = s;
}

void Animator::Update(float dt)
{
	timer -= dt;
	if (timer <= 0)
	{
		timer = speed;
		Next();
	}
}

void Animator::Animate(int x, int y, SDL_RendererFlip flip)
{
	Engine::GetInstance().render.get()->DrawTexture(animations[currentAnimation].sprites[currentSprite].texture, x, y, flip, &animations[currentAnimation].GetSpriteRect(currentSprite), 1, 0, animations[currentAnimation].sprites[currentSprite].size.getX()/2, animations[currentAnimation].sprites[currentSprite].size.getY() / 2);
}
