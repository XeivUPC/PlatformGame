#pragma once
#include "Module.h"
#include "Textures.h"
#include "SDL2/SDL.h"
#include "Vector2D.h"

class TextGenerator : public Module
{
private:
	SDL_Texture* fontTexture = NULL;
	Vector2D textureSize;
	Vector2D spriteSize;
public:
	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();
};