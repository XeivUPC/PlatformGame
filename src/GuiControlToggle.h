#pragma once
#include "GuiControl.h"
#include "Vector2D.h"
class GuiControlToggle : public GuiControl
{
public:
	GuiControlToggle(SDL_Rect b, SDL_Texture* tex, bool isOn = true);
	virtual ~GuiControlToggle();

	// Called each loop iteration
	bool Update(float dt);
	void TurnOn();
	void TurnOff();
	void SetStatus(bool status);
private:

	bool isOn;
};

