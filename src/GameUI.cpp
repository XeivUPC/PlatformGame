#include "GameUI.h"
#include "Render.h"
#include "Engine.h"
#include "Enemy.h"
#include "LevelSection.h"
#include "Window.h"
#include "Player.h"
#include "LevelManager.h"
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
	Engine::GetInstance().text->Write(std::to_string(player->coins.GetAmount()).c_str(), -Engine::GetInstance().render->camera.x + 13, -Engine::GetInstance().render->camera.y + 8);
	Engine::GetInstance().text->Write("-gold-", -Engine::GetInstance().render->camera.x + 6, -Engine::GetInstance().render->camera.y);
	rect = { 96,80,16,16 };
	Engine::GetInstance().render->DrawTexture(texture, -Engine::GetInstance().render->camera.x + 64, -Engine::GetInstance().render->camera.y, SDL_FLIP_NONE, &rect);
	rect = { 24,80,8,16 };
	Engine::GetInstance().render->DrawTexture(texture, -Engine::GetInstance().render->camera.x + 80, -Engine::GetInstance().render->camera.y, SDL_FLIP_NONE, &rect);
	Engine::GetInstance().text->Write("-item-", -Engine::GetInstance().render->camera.x + 81, -Engine::GetInstance().render->camera.y);
	Engine::GetInstance().text->Write(std::to_string(player->magic.GetAmount()).c_str(), -Engine::GetInstance().render->camera.x + 89, -Engine::GetInstance().render->camera.y + 8, {58,191,237,255});
	Engine::GetInstance().text->Write("-life-", -Engine::GetInstance().render->camera.x + 160, -Engine::GetInstance().render->camera.y);
	int remainingHealth = player->playerHealth.GetCurrentHealth();
	for (size_t i = 0; i < (player->playerHealth.GetBaseHealth()/2); i++)
	{
		if (remainingHealth >= 2)
		{
			rect = {32,88,8,8};
			remainingHealth -= 2;
		}
		else if (remainingHealth == 1)
		{
			rect = { 40,88,8,8 };
			remainingHealth = 0;
		}
		else
			rect = { 64,88,8,8 };
		Engine::GetInstance().render->DrawTexture(texture, -Engine::GetInstance().render->camera.x + 161 + 9*i, -Engine::GetInstance().render->camera.y + 7, SDL_FLIP_NONE, &rect);
	}
	Engine::GetInstance().text->Write("-boss-", -Engine::GetInstance().render->camera.x + 350, -Engine::GetInstance().render->camera.y);
	rect = { 64,88,8,8 };
	for (size_t i = 0; i < 10; i++)
	{
		Engine::GetInstance().render->DrawTexture(texture, -Engine::GetInstance().render->camera.x + 309 + 9 * i, -Engine::GetInstance().render->camera.y + 7, SDL_FLIP_NONE, &rect);
	}
	remainingHealth = Engine::GetInstance().levelManager->GetCurrentSection()->boss->enemyHealth.GetCurrentHealth();
	for (size_t i = 0; i < (Engine::GetInstance().levelManager->GetCurrentSection()->boss->enemyHealth.GetBaseHealth() / 2); i++)
	{
		if (remainingHealth >= 2)
		{
			rect = { 48,88,8,8 };
			remainingHealth -= 2;
		}
		else if (remainingHealth == 1)
		{
			rect = { 56,88,8,8 };
			remainingHealth = 0;
		}
		else
			rect = { 64,88,8,8 };
		Engine::GetInstance().render->DrawTexture(texture, -Engine::GetInstance().render->camera.x + 390 - 9 * i, -Engine::GetInstance().render->camera.y + 7, SDL_FLIP_NONE, &rect);
	}
	
	Engine::GetInstance().render->UnlockLayer();
}

void GameUI::CleanUp()
{
}

