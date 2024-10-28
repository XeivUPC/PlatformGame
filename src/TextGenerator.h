#pragma once
#include "Module.h"
#include "Render.h"
#include "Textures.h"
#include "Engine.h"
#include <vector>
#include "Vector2D.h"
#include "SDL2/SDL.h"

class TextGenerator : public Module
{
private:
	SDL_Texture* fontTexture = NULL;
	Vector2D size;
	Vector2D count;
	std::vector<int> charReferences;
	SDL_Rect GetCharRect(char c);
public:
	TextGenerator();

	virtual ~TextGenerator();

	bool Awake();

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();
	void Write(const char* text, int x, int y, SDL_Color color = {255,255,255,255});
};