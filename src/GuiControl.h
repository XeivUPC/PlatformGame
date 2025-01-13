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

	// Constructor
	GuiControl(GuiControlType type, int id);

	// Constructor
	GuiControl(GuiControlType type, SDL_Rect bounds, const char* text);

	// Called each loop iteration
	virtual bool Update(float dt);
	virtual void Render();

	// 
	void SetTexture(SDL_Texture* tex);

	// 
	void SetObserver(Module* module);

	// 
	void NotifyObserver();

	bool IsInBounds(Vector2D point);

	GuiControlState CurrentState();

	int id;
	GuiControlType type;

	std::string text;       // Control text (if required)
	SDL_Rect bounds;        // Position and size
	
	SDL_Color color;		// Color
	SDL_Color textColor;    // Text color

	SDL_Texture* texture;   // Texture atlas reference
	SDL_Rect section;       // Texture atlas base section

	Module* observer;       // Observer 
private:
	GuiControlState state;
};