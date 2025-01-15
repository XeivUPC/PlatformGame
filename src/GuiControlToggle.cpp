#include "GuiControlToggle.h"
#include "Engine.h"

GuiControlToggle::GuiControlToggle(SDL_Rect b, SDL_Texture* tex, bool isOn) : GuiControl(GuiControlType::TOGGLE, b, tex)
{
	this->isOn = isOn;
}

GuiControlToggle::~GuiControlToggle()
{
}

bool GuiControlToggle::Update(float dt)
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
		if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
			SetStatus(!isOn);
		}
	}
	if (isOn) {
		state = GuiControlState::PRESSED;
	}

	return true;
}

void GuiControlToggle::TurnOn()
{
	isOn = true;
}

void GuiControlToggle::TurnOff()
{
	isOn = false;
}

bool GuiControlToggle::IsOn()
{
	return isOn;
}

void GuiControlToggle::SetStatus(bool status)
{
	if (status)
		TurnOn();
	else {
		TurnOff();
	}
	NotifyObserver();
}
