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

class Animation
{
	friend class Animator;
private:
	const char* name;
	std::vector <Sprite> sprites;
protected:
	SDL_Rect GetSpriteRect(int spriteIndex);
	SDL_Texture* GetTexture(int spriteIndex);
	int GetCount(int spriteIndex);
public:
	Animation(const char* n, std::vector<Sprite> s);
};

class Animator
{
private:
	std::unordered_map<const char*, Animation> animations;
	int customSpriteRangeStart = -1, customSpriteRangeEnd = -1;
	const char* currentAnimation = "";
	int currentSprite = 0;
	bool loop = false;
	float speed = 1;
	float timer = 0;
	void Next();

public:
	Animator(std::vector<Animation> anims, float s);
	void SelectAnimation(const char* animName, bool l);
	void SelectAnimation(const char* animName, bool l, int indexInit, int indexEnd);
	void SetSpeed(float s);
	void Update(float dt);
	void Animate(int x, int y, SDL_RendererFlip flip);
};

//class Animation
//{
//private:
//	const char* index;
//	std::vector<Sprite> sprites;
//	int count = 0;
//	
//public:
//	Animation(const char* name, SDL_Texture* t, std::vector<Sprite> s);
//	bool IsSame(const char* name);
//	int GetCount();
//	SDL_Texture* GetTexture();
//};
//
//class Animator
//{
//private:
//	std::vector<Animation> animations;
//	int currentSpriteIndex = 0;
//	int currentAnimationIndex = 0;
//	float animationSpeed = 1;
//	float timer = 0;
//	bool loop = false;
//	int GetAnimation(const char* name);
//	void NextStep();
//public:
//	Animator(std::vector<Animation> anims, float speed);
//	void Update(float dt);
//	int GetActualSprite();
//	int SetAnimationSpeed(float speed);
//	void SelectAnimation(const char* name, bool l);
//	void ModifyIndex(int i);
//	void Animate(Vector2D position, SDL_RendererFlip flip);
//};