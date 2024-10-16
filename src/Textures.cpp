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
	for (const auto& texture : textures) {
		LOG("Freeing texture");
		SDL_DestroyTexture(texture.second);
	}
	textures.clear();

	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const Textures::Load(std::string path)
{

	if (textures.count(path) != 0) {
		//LOG("Texture is already loaded: %s", path.c_str());
		return textures[path];
	}


	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(path.c_str());

	if (surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = LoadSurface(surface);
		if(texture!=nullptr)
			textures[path] = texture;
		SDL_FreeSurface(surface);
	}

	return texture;
}

// Unload texture
bool Textures::UnLoad(SDL_Texture* texture)
{
	for (const auto& _texture : textures) {
		if (_texture.second == texture) {
			LOG("Unloading texture");
			SDL_DestroyTexture(texture);
			return true;
		}
	}

	return false;
}

// Translate a surface into a texture
SDL_Texture* const Textures::LoadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance().render
		
		
		->renderer, surface);

	if (texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}

	return texture;
}

// Retrieve size of a texture
void Textures::GetSize(const SDL_Texture* texture, int& width, int& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*)&width, (int*)&height);
}
