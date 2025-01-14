#pragma once

#include "Module.h"
#include "GuiControl.h"

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
	GuiControl* CreateGuiControl(GuiControlType type, SDL_Rect bounds, SDL_Texture* texture, Module* observer);

public:

	SDL_Texture* texture;

};

