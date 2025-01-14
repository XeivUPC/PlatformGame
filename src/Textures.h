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

	void GetSize(const SDL_Texture* texture, int& width, int& height) const;
	SDL_Texture* GetTexture(std::string textureID);
	void CreateTexture(std::string path, std::string textureID);
	void DeleteTexture(std::string textureID);

private:
	SDL_Texture* const LoadSurface(SDL_Surface* surface);
	bool IsTextureLoaded(std::string textureID);
	std::unordered_map<std::string, SDL_Texture*> textureData;
};