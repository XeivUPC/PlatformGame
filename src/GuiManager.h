#pragma once

#include "Module.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

#include <list>

class GuiManager : public Module
{
public:

	// Constructor
	GuiManager();

	// Destructor
	virtual ~GuiManager();

	// Called before the first frame
	 bool Start();

	

	// Additional methods
	GuiControl* CreateGuiControlButton(SDL_Rect bounds, SDL_Texture* texture, Module* observer);
	GuiControl* CreateGuiControlToggle(SDL_Rect bounds, SDL_Texture* texture, bool status, Module* observer);
	GuiControl* CreateGuiControlSlider(SDL_Rect bounds, SDL_Texture* texture, GuiControlButton* t, float v, float mn, float mx, Module* observer);

public:

	SDL_Texture* texture;

};

