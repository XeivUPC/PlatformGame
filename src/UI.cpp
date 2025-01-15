#include "UI.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Vector2D.h"
#include "Window.h"

UI::UI(Module* ModuleAt)
{
	moduleAt = ModuleAt;
	texture = Engine::GetInstance().textures->GetTexture("UITexture");
}

UI::~UI()
{
}

void UI::Init(float dt)
{
}

void UI::Update(float dt)
{
}

void UI::Render()
{
}

void UI::CleanUp()
{
}

void UI::SetInteractable(bool p)
{
	isInteractable = p;
}
