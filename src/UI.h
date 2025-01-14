#pragma once
#include "SDL2/SDL.h"
class GuiControlButton;
class Module;
class UI
{
protected:
	Module* moduleAt;
	SDL_Texture* texture;
public:
	UI(Module* ModuleAt);
	~UI();
	virtual void Update(float dt);
	virtual void Render();
	virtual void CleanUp();
};