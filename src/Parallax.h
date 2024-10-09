#pragma once
#include "Module.h"
#include "SDL2/SDL.h"
#include <vector>
#include "Vector2D.h"


struct ParallaxLayer {
	SDL_Texture* texture;
	float speed;
	Vector2D offset;
};


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
	void LoadTexture(std::string path, float speed);
private:

	float Wrap(float value, float max);
	std::vector<ParallaxLayer> ParallaxLayers;

	float lastCameraX = 0;
};