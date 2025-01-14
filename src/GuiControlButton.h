#pragma once

#include "GuiControl.h"
#include "Vector2D.h"

class GuiControlButton : public GuiControl
{

public:
	GuiControlButton(GuiControlType t, SDL_Rect b, SDL_Texture* tex);
	virtual ~GuiControlButton();

	// Called each loop iteration
	bool Update(float dt);
};

