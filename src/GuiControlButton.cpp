#include "GuiControlButton.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"
#include "TextGenerator.h"

GuiControlButton::GuiControlButton(GuiControlType t, SDL_Rect b, SDL_Texture* tex) : GuiControl(GuiControlType::BUTTON, b, tex)
{
}

GuiControlButton::~GuiControlButton()
{}

bool GuiControlButton::Update(float dt)
{
	if (!isEnabled)
	{
		state = GuiControlState::DISABLED;
		return true;
	}
	else state = GuiControlState::NORMAL;
	
	if (IsInBounds(Engine::GetInstance().input->GetMousePosition()))
	{
		state = GuiControlState::FOCUSED;
		if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			state = GuiControlState::PRESSED;
		if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			NotifyObserver();
	}
	GuiControl::Update(dt);
	return true;
}
