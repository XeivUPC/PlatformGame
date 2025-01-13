#include "GuiControl.h"
#include "Engine.h"
#include "Input.h"
#include "Render.h"

GuiControl::GuiControl(GuiControlType type, int id) : type(type), id(id), state(GuiControlState::NORMAL) {}
GuiControl::GuiControl(GuiControlType type, SDL_Rect bounds, const char* text) :
	type(type),
	state(GuiControlState::NORMAL),
	bounds(bounds),
	text(text)
{
	color.r = 255; color.g = 255; color.b = 255;
	texture = NULL;
}

bool GuiControl::Update(float dt)
{
	Render();
	return true;
}

void GuiControl::Render()
{
}

void GuiControl::SetTexture(SDL_Texture* tex)
{
	texture = tex;
	section = { 0, 0, 0, 0 };
}

void GuiControl::SetObserver(Module* module)
{
	observer = module;
}

void GuiControl::NotifyObserver()
{
	observer->OnGuiMouseClickEvent(this);
}

bool GuiControl::IsInBounds(Vector2D point)
{
	if (point.getX() > bounds.x && point.getX() < bounds.x + bounds.w && point.getY() > bounds.y && point.getY() < bounds.y + bounds.h)
		return true;
	return false;
}

GuiControlState GuiControl::CurrentState()
{
	Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();
	if (state == GuiControlState::DISABLED)
		return GuiControlState::DISABLED;
	if (IsInBounds(mousePos))
	{
		state = GuiControlState::FOCUSED;
		if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
			state = GuiControlState::PRESSED;
		}

		if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
			NotifyObserver();
			state = GuiControlState::SELECTED;
		}
	}
	else
		state = GuiControlState::NORMAL;
	return state;
}

