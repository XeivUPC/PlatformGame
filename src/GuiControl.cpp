#include "GuiControl.h"
#include "Engine.h"
#include "Input.h"
#include "Render.h"
#include "GuiManager.h"

GuiControl::GuiControl(GuiControlType t, SDL_Rect b, SDL_Texture* tex)
{
	type = t;
	bounds = b;
	texture = tex;
	isEnabled = true;
	state = GuiControlState::NORMAL;
	for (size_t i = 0; i < 4; i++)
	{
		rectangles.emplace_back(SDL_Rect{ 0,0,0,0 });
	}
}

GuiControl::~GuiControl()
{
}

bool GuiControl::Update(float dt)
{
	Render();
	return true;
}

void GuiControl::Render()
{
	Engine::GetInstance().render->SelectLayer(Engine::GetInstance().render->Layer6);
	Engine::GetInstance().render->DrawTexture(texture, -Engine::GetInstance().render->camera.x+bounds.x, -Engine::GetInstance().render->camera.y + bounds.y, SDL_FLIP_NONE, &rectangles[(int)state], {255,255,255,255});
}

void GuiControl::CleanUp()
{
	rectangles.clear();
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

void GuiControl::SetRectangle(SDL_Rect rect, GuiControlState state)
{
	rectangles[(int)state] = rect;
}

void GuiControl::Enable()
{
	isEnabled = true;
}

void GuiControl::Disable()
{
	isEnabled = false;
}

GuiControlState GuiControl::CurrentState()
{
	return state;
}

