#pragma once

#include "Module.h"
#include <unordered_map>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"

class Textures : public Module
{
public:

	Textures();

	// Destructor
	virtual ~Textures();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	// Load Texture
	SDL_Texture* const Load(std::string path);
	SDL_Texture* const LoadSurface(SDL_Surface* surface);
	bool UnLoad(SDL_Texture* texture);
	void GetSize(const SDL_Texture* texture, int& width, int& height) const;

public:
	std::unordered_map<std::string,SDL_Texture*> textures;

};