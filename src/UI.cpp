#include "UI.h"
#include "Engine.h"
#include "Textures.h"

UI::UI()
{
}

UI::~UI()
{
}

bool UI::Awake()
{
	return false;
}

bool UI::Start()
{
	UITexture = Engine::GetInstance().textures->Load("Assets/Textures/UI.png");
	return false;
}

bool UI::PreUpdate()
{
	return false;
}

bool UI::Update(float dt)
{
	return false;
}

bool UI::PostUpdate()
{
	return false;
}

bool UI::CleanUp()
{
	return false;
}
