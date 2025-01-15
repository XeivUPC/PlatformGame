#include "IntroUI.h"
#include "Textures.h"
#include "Engine.h"
#include "Render.h"

IntroUI::IntroUI(Module* ModuleAt) : UI(ModuleAt)
{
	bg_texture = Engine::GetInstance().textures->GetTexture("Intro");
}

IntroUI::~IntroUI()
{
}

void IntroUI::Update(float dt)
{
	Render();
}

void IntroUI::Render()
{
	Engine::GetInstance().render->LockLayer(Render::Layer6);
	SDL_Rect rect = { 0,0,427,239 };
	Engine::GetInstance().render->DrawTexture(bg_texture, 0, 0, SDL_FLIP_NONE, &rect);
}

void IntroUI::CleanUp()
{
}
