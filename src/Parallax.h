#pragma once
#include "Module.h"
#include "SDL2/SDL.h"
#include <vector>

class  Parallax : public Module
{
private:
	std::vector<SDL_Texture*> ParallaxLayers;
	float ParallaxFactor;
public:
	bool Awake()override;
	bool PreUpdate()override;
	bool Update(float dt)override;
	bool CleanUp()override;
};