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

	int extraData;
};

class AnimationData
{
	friend class Animator;
private:
	std::string name = "";
	std::vector <Sprite> sprites;
protected:
	SDL_Rect GetSpriteRect(int spriteIndex);
	SDL_Texture* GetTexture(int spriteIndex);
	int GetCount(int spriteIndex);
public:
	void AddSprite(Sprite sprite);
	void AddSprite(Sprite sprite, int extraData);
	AnimationData(std::string n, std::vector<Sprite> s);
	AnimationData(std::string n);
	AnimationData();
	~AnimationData();

};

class Animator
{
private:
	std::unordered_map<std::string, AnimationData> animations;
	int customSpriteRangeStart = 0, customSpriteRangeEnd = 0;
	std::string currentAnimation = "";
	int currentSprite = 0;
	bool loop = false;
	float speed = 1;
	float timer = 0;
	void Next();
	bool isPlaying=true;
	bool canDraw=true;

public:
	Animator();
	~Animator();
	Animator(std::vector<AnimationData> anims, float s);
	void AddAnimation(AnimationData anim);
	void SelectAnimation(std::string animName, bool l);
	void SelectAnimation(std::string animName, bool l, int indexInit, int indexEnd);
	void SetSpeed(float s);
	void Update(float dt);
	void Animate(int x, int y, SDL_RendererFlip flip);

	void SetIfPlaying(bool isPlaying);
	bool IsPlaying();

	void SetIfCanDraw(bool canDraw);
	bool CanDraw();

	std::string GetCurrentAnimationName();
	AnimationData GetCurrentAnimation();
	Sprite GetCurrentAnimationSprite();
};