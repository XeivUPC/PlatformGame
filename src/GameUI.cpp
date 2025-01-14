#include "GameUI.h"
#include "Render.h"
#include "Engine.h"
#include "Window.h"
#include "Player.h"
#include "TextGenerator.h"

GameUI::GameUI(Module* ModuleAt, Player* p): UI(ModuleAt)
{
	player = p;
}

GameUI::~GameUI()
{
}

void GameUI::Update(float dt)
{
	Render();
}

void GameUI::Render()
{
	Engine::GetInstance().render->LockLayer(Render::Layer5);
	Engine::GetInstance().render->DrawRectangle({ 0,0, Engine::GetInstance().window->width,16 }, 0, 0, 0, 255, true, false);
	SDL_Rect rect = {0,80,16,16};
	Engine::GetInstance().render->DrawTexture(texture, -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y, SDL_FLIP_NONE, &rect);
	Engine::GetInstance().text->Write(std::to_string(player->coins.GetAmount()).c_str(), -Engine::GetInstance().render->camera.x + 16, -Engine::GetInstance().render->camera.y + 8);
	Engine::GetInstance().render->UnlockLayer();
}

void GameUI::CleanUp()
{
}

