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
	sprite.extraData = -1;
	sprites.push_back(sprite);
}

void AnimationData::AddSprite(Sprite sprite, int extraData)
{
	sprite.extraData = extraData;
	sprites.push_back(sprite);
}

AnimationData::AnimationData(std::string n, std::vector<Sprite> s)
{
	name = n;
	sprites = s;
}

AnimationData::AnimationData(std::string n)
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
				animationEnded = true;
			}
			else
			{
				currentSprite = animations[currentAnimation].sprites.size() - 1;
				animationEnded = true;
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

void Animator::SelectAnimation(std::string animName, bool l)
{
	if (currentAnimation == animName)
		return;
	animationEnded = false;
	currentAnimation = animName;
	loop = l;
	customSpriteRangeStart = 0;
	customSpriteRangeEnd = animations[currentAnimation].sprites.size();
	currentSprite = 0;
}

void Animator::SelectAnimation(std::string animName, bool l, int indexInit, int indexEnd)
{
	if (currentAnimation == animName)
		return;
	animationEnded = false;
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
	if (!isPlaying)
		return;
	timer -= dt;
	if (timer <= 0)
	{
		timer = speed;
		Next();
	}
}

void Animator::Animate(int x, int y, SDL_RendererFlip flip)
{	
	if (!canDraw)
		return;
	SDL_Rect rect = animations[currentAnimation].GetSpriteRect(currentSprite);
	!Engine::GetInstance().render->DrawTexture(animations[currentAnimation].sprites[currentSprite].texture, x, y, flip, &rect);
}

void Animator::SetIfPlaying(bool isPlaying)
{
	this->isPlaying = isPlaying;
}

bool Animator::IsPlaying()
{
	return isPlaying;
}

void Animator::SetIfCanDraw(bool canDraw)
{
	this->canDraw = canDraw;
}

bool Animator::CanDraw()
{
	return canDraw;
}

bool Animator::AnimationEnded(std::string animName)
{
	if (currentAnimation == animName && animationEnded)
	{
		return true;
	}
	return false;
}

bool Animator::OnFrame(int id)
{
	if (currentSprite == id)return true;
	return false;
}

std::string Animator::GetCurrentAnimationName()
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
