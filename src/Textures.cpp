#include "Engine.h"
#include "Render.h"
#include "Textures.h"
#include "Log.h"


Textures::Textures() : Module()
{
	name = "textures";
}

// Destructor
Textures::~Textures()
{}

// Called before render is available
bool Textures::Awake()
{
	LOG("Init Image library");
	bool ret = true;

	// Load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool Textures::Start()
{
	LOG("start textures");
	bool ret = true;
	return ret;
}

// Called before quitting
bool Textures::CleanUp()
{
	LOG("Freeing textures and Image library");

	for (; textureData.size() != 0;)
	{
		DeleteTexture(textureData.begin()->first);
	}
	textureData.clear();

	IMG_Quit();
	return true;
}



// Translate a surface into a texture
SDL_Texture* const Textures::LoadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance().render->renderer, surface);

	if (texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}

	return texture;
}

bool Textures::IsTextureLoaded(std::string textureID)
{
	return textureData.find(textureID) != textureData.end();
}

// Retrieve size of a texture
void Textures::GetSize(const SDL_Texture* texture, int& width, int& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*)&width, (int*)&height);
}

SDL_Texture* Textures::GetTexture(std::string textureID)
{
	if (!IsTextureLoaded(textureID))
	{
		printf("Texture File Not Loaded");
		return nullptr;
	}
	return textureData[textureID];
}

void Textures::CreateTexture(std::string path, std::string textureID)
{
	if (IsTextureLoaded(textureID))
	{
		printf("Texture File Already Loaded");
		return;
	}
	SDL_Surface* surface = IMG_Load(path.c_str());
	SDL_Texture* texture = nullptr;
	if (surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else {
		texture = LoadSurface(surface);
		if (texture != nullptr)
		{
			textureData[textureID] = texture;	
		}
		else {
			LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
		}
		SDL_FreeSurface(surface);
	}

	
}

void Textures::DeleteTexture(std::string textureID)
{
	if (!IsTextureLoaded(textureID))
	{
		printf("Texture File Not Loaded");
		return;
	}
	SDL_DestroyTexture(textureData[textureID]);
	textureData.erase(textureID);
}
