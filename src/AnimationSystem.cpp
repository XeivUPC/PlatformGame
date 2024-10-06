#include "AnimationSystem.h"
#include "Timer.h"
#include "Render.h"
#include "Textures.h"
#include "Engine.h"


SDL_Rect AnimationData::GetSpriteRect(int spriteIndex)
{
	SDL_Rect spriteRect = SDL_Rect();
	
	spriteRect.w = sprites[spriteIndex].size.getX();
	spriteRect.h = sprites[spriteIndex].size.getY();

	spriteRect.x = sprites[spriteIndex].index.getX() * spriteRect.w;
	spriteRect.y = sprites[spriteIndex].index.getY() * spriteRect.h;
	return spriteRect;
}

SDL_Texture* AnimationData::GetTexture(int spriteIndex)
{
	return sprites[spriteIndex].texture;
}

int AnimationData::GetCount(int spriteIndex)
{
	return sprites.size();
}

void AnimationData::AddSprite(Sprite sprite)
{
	sprites.push_back(sprite);
}

AnimationData::AnimationData(const char* n, std::vector<Sprite> s)
{
	name = n;
	sprites = s;
}

AnimationData::AnimationData(const char* n)
{
	name = n;
}

AnimationData::AnimationData()
{
}

AnimationData::~AnimationData()
{
	sprites.clear();
}


void Animator::Next()
{
	if (customSpriteRangeStart != -1 && customSpriteRangeEnd != -1)
	{
		if (currentSprite < animations[currentAnimation].sprites.size() - 1)
			currentSprite++;
		else
		{
			if (loop)
			{
				currentSprite = 0;
			}
			else
			{
				currentSprite = animations[currentAnimation].sprites.size() - 1;
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

Animator::Animator()
{

}

Animator::~Animator()
{
	animations.clear();
}

Animator::Animator(std::vector<AnimationData> anims, float s)
{
	for (int i = 0; i < anims.size(); i++)
	{
		AddAnimation(anims[i]);
	}
	speed = s;
}

void Animator::AddAnimation(AnimationData anim)
{
	animations[anim.name] = anim;
}

void Animator::SelectAnimation(const char* animName, bool l)
{
	if (currentAnimation == animName)
		return;

	currentAnimation = animName;
	loop = l;
	customSpriteRangeStart = 0;
	customSpriteRangeEnd = animations[currentAnimation].sprites.size();
	currentSprite = 0;
}

void Animator::SelectAnimation(const char* animName, bool l, int indexInit, int indexEnd)
{
	if (currentAnimation == animName)
		return;


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
	SDL_Rect rect = animations[currentAnimation].GetSpriteRect(currentSprite);
	Engine::GetInstance().render.get()->DrawTexture(animations[currentAnimation].sprites[currentSprite].texture, x, y, flip, &rect, 1, 0);
}

const char* Animator::GetCurrentAnimationName()
{
	return currentAnimation;
}

AnimationData Animator::GetCurrentAnimation()
{
	return animations[GetCurrentAnimationName()];
}

Sprite Animator::GetCurrentAnimationSprite()
{
	return GetCurrentAnimation().sprites[currentSprite];
}
