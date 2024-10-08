#pragma once
#include "Module.h"
#include "SDL2/SDL.h"
#include <vector>

class  Parallax : public Module
{
public:
	Parallax();
	virtual ~Parallax();
	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void LoadTexture(std::string path);
private:
	std::vector<SDL_Texture*> ParallaxLayers;
	int count = 0;
	float parallaxFactor;
	float offset;
	int parallaxStep = 0;

};