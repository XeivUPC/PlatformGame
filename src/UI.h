#pragma once
#include "Module.h"
#include "SDL2/SDL.h"
class Stats
{

};

class UI : public Module
{
public:
	UI();
	virtual ~UI();
	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	Stats topUI;
private:
	SDL_Texture* UITexture = NULL;
};