#pragma once

#include "Input.h"
#include "Render.h"
#include "Module.h"
#include "Vector2D.h"

enum class GuiControlType
{
	BUTTON,
	TOGGLE,
	CHECKBOX,
	SLIDER,
	SLIDERBAR,
	COMBOBOX,
	DROPDOWNBOX,
	INPUTBOX,
	VALUEBOX,
	SPINNER
};

enum class GuiControlState
{
	DISABLED,
	NORMAL,
	FOCUSED,
	PRESSED,
	SELECTED
};

class GuiControl
{
public:

	GuiControl(GuiControlType t, SDL_Rect b, SDL_Texture* tex);
	~GuiControl();
	virtual bool Update(float dt);
	virtual void Render();
	virtual void CleanUp();

	void SetObserver(Module* module);

	void NotifyObserver();

	bool IsInBounds(Vector2D point);

	void SetRectangle(SDL_Rect rect, GuiControlState state);

	void Enable();

	void Disable();

	void SetState(GuiControlState state);
	GuiControlState CurrentState();

	GuiControlType type;
	
	SDL_Rect bounds;

	SDL_Texture* texture = nullptr;

	Module* observer = nullptr;

	bool isEnabled;

protected:
	GuiControlState state;
	std::vector<SDL_Rect> rectangles;
};