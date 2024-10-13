#pragma once
#include "Vector2D.h"
#include "SDL2/SDL.h"
#include <vector>
#include <unordered_map>

struct Sprite
{
	SDL_Texture* texture;
	Vector2D index;
	Vector2D size;
};

class AnimationData
{
	friend class Animator;
private:
	const char* name = "";
	std::vector <Sprite> sprites;
protected:
	SDL_Rect GetSpriteRect(int spriteIndex);
	SDL_Texture* GetTexture(int spriteIndex);
	int GetCount(int spriteIndex);
public:
	void AddSprite(Sprite sprite);
	AnimationData(const char* n, std::vector<Sprite> s);
	AnimationData(const char* n);
	AnimationData();
	~AnimationData();

};

class Animator
{
private:
	std::unordered_map<const char*, AnimationData> animations;
	int customSpriteRangeStart = 0, customSpriteRangeEnd = 0;
	const char* currentAnimation = "";
	int currentSprite = 0;
	bool loop = false;
	float speed = 1;
	float timer = 0;
	void Next();
	bool isPlaying=true;

public:
	Animator();
	~Animator();
	Animator(std::vector<AnimationData> anims, float s);
	void AddAnimation(AnimationData anim);
	void SelectAnimation(const char* animName, bool l);
	void SelectAnimation(const char* animName, bool l, int indexInit, int indexEnd);
	void SetSpeed(float s);
	void Update(float dt);
	void Animate(int x, int y, SDL_RendererFlip flip);
	void SetIfPlaying(bool isPlaying);
	const char* GetCurrentAnimationName();
	AnimationData GetCurrentAnimation();
	Sprite GetCurrentAnimationSprite();
};