#include "UI.h"
#include "Engine.h"
#include "Textures.h"
#include "TextGenerator.h"
#include "Window.h"
#include "Input.h"
#include "GameScene.h"

UI::UI()
{
}

UI::~UI()
{
}

bool UI::Awake()
{
	return true;
}

bool UI::Start()
{
	UITexture = Engine::GetInstance().textures->Load("Assets/Textures/UI.png");
	topUI.Activate();
	pauseMenuUI.Deactivate();
	return true;
}

bool UI::PreUpdate()
{
	return true;
}

bool UI::Update(float dt)
{
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		topUI.Deactivate();
	}
	topUI.Render();
	pauseMenuUI.Render();
	dialogueUI.Render();
	return true;
}

bool UI::PostUpdate()
{
	return true;
}

bool UI::CleanUp()
{
	return true;
}

SDL_Texture* UI::GetTexture()
{
	return UITexture;
}

void UI::DrawOnOption(SDL_Rect* rect, int option, Vector2D scale, Vector2D count, Vector2D offset, Constraint constraint)
{
}

void UIGroup::Activate()
{
	isActive = true;
}

void UIGroup::Deactivate()
{
	isActive = false;
}

void TopUI::OrbDrawAndLogic(Health counter, int factor, int fullIndex, int halfIndex, int emptyIndex, Vector2D offset)
{
	SDL_Rect orb;
	orb.y = 8;
	orb.w = 8;
	orb.h = 8;
	int step = 0;
	while (counter.GetBaseHealth() > 0)
	{
		if (counter.GetCurrentHealth() >= factor)
		{
			//Full orb Rect
			orb.x = 8 * fullIndex;
			step++;
			counter.Hurt(factor);
		}
		else if (counter.GetCurrentHealth() != 0)
		{
			//Half orb Rect
			orb.x = 8 * halfIndex;
			step++;
			counter.Hurt(factor);
		}
		else
		{
			//Empty orb Rect
			orb.x = 8 * emptyIndex;
			step++;
		}

		//Engine::GetInstance().render->DrawTexture(Engine::GetInstance().ui->GetTexture(), -Engine::GetInstance().render->camera.x + step * 8 + offset.getX() + step, -Engine::GetInstance().render->camera.y + offset.getY()+ offset.getY(), SDL_FLIP_NONE, &orb);
		counter.ModifyBaseHealth(counter.GetBaseHealth() - factor);
	}
		
}

void TopUI::Render()
{
	Engine::GetInstance().render->LockLayer(Render::RenderLayers::Layer5);

	SDL_Rect top{ -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y, Engine::GetInstance().window->width, 16  };
	Engine::GetInstance().render->DrawRectangle(top,0,0,0);

	if (!isActive) {
		Engine::GetInstance().render->UnlockLayer();
		return;
	}

	OrbDrawAndLogic(Engine::GetInstance().game_scene->player->playerHealth, 2, 4, 5, 8, {152,4});
	SDL_Rect chest{ 0, 0, 16, 16 };
	//Engine::GetInstance().render->DrawTexture(Engine::GetInstance().ui->GetTexture(), -Engine::GetInstance().render->camera.x + 0, -Engine::GetInstance().render->camera.y, SDL_FLIP_NONE, &chest);
	SDL_Rect potion{ 16, 0, 16, 16 };
	//Engine::GetInstance().render->DrawTexture(Engine::GetInstance().ui->GetTexture(), -Engine::GetInstance().render->camera.x + 80, -Engine::GetInstance().render->camera.y, SDL_FLIP_NONE, &potion);
	Engine::GetInstance().text->Write("-gold-    -item-    -life-                  -boss-", -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y);
	Engine::GetInstance().text->Write(std::to_string(Engine::GetInstance().game_scene->player->coins.GetAmount()).c_str(), -Engine::GetInstance().render->camera.x + 16, -Engine::GetInstance().render->camera.y  + 8);
	Engine::GetInstance().text->Write(std::to_string(Engine::GetInstance().game_scene->player->magic.GetAmount()).c_str(), -Engine::GetInstance().render->camera.x + 96, -Engine::GetInstance().render->camera.y  + 8);

	Engine::GetInstance().render->UnlockLayer();
}


void Pause::Render()
{
	if (!isActive)
		return;

	Engine::GetInstance().render->LockLayer(Render::RenderLayers::Layer6);
	SDL_Rect pause{ -Engine::GetInstance().render->camera.x + 64 , -Engine::GetInstance().render->camera.y + 32 , 272, 176};
	Engine::GetInstance().render->DrawRectangle(pause, 0, 0, 0);
	Engine::GetInstance().render->DrawRectangle(pause, 255, 255, 255, 255, false);

	Engine::GetInstance().text->Write("-PAUSE-", 176, 40, { 44,152,213,255 });
	pause.x = 112;
	pause.y = 0;
	pause.w = 16;
	pause.h = 16;
	//Engine::GetInstance().render->DrawTexture(Engine::GetInstance().ui->GetTexture(), -Engine::GetInstance().render->camera.x + 65, -Engine::GetInstance().render->camera.y + 33, SDL_FLIP_NONE, &pause);
	pause.x = 128;
	//Engine::GetInstance().render->DrawTexture(Engine::GetInstance().ui->GetTexture(), -Engine::GetInstance().render->camera.x + 319, -Engine::GetInstance().render->camera.y + 33, SDL_FLIP_NONE, &pause);
	pause.x = 144;
	//Engine::GetInstance().render->DrawTexture(Engine::GetInstance().ui->GetTexture(), -Engine::GetInstance().render->camera.x + 319, -Engine::GetInstance().render->camera.y + 191, SDL_FLIP_NONE, &pause);
	pause.x = 160;
	//Engine::GetInstance().render->DrawTexture(Engine::GetInstance().ui->GetTexture(), -Engine::GetInstance().render->camera.x + 65, -Engine::GetInstance().render->camera.y + 191, SDL_FLIP_NONE, &pause);

	Engine::GetInstance().render->UnlockLayer();
}

void Dialogue::Render()
{
	if (!isActive)
		return;
}

