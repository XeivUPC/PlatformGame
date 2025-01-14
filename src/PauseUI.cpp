#include "PauseUI.h"
#include "GuiManager.h"
#include "Engine.h"
#include "Window.h"
#include "Textures.h"
#include "GuiControlButton.h"


PauseUI::PauseUI(Scene* GameAt)
{
	gameAt = GameAt;
	texture = Engine::GetInstance().textures->Load("Assets/Textures/UI1.png");
	pauseButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControl(GuiControlType::BUTTON, "", { Engine::GetInstance().window->width - 20,10,10,10 }, texture, (Module*)gameAt);
	pauseButton->SetRectangle({ 0,0,16,16 }, GuiControlState::NORMAL);
	pauseButton->SetRectangle({ 16,0,16,16 }, GuiControlState::FOCUSED);
	pauseButton->SetRectangle({ 32,0,16,16 }, GuiControlState::PRESSED);
	pauseButton->SetRectangle({ 0,0,16,16 }, GuiControlState::DISABLED);
	resumeButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControl(GuiControlType::BUTTON, "", { Engine::GetInstance().window->width - 20,30,10,10 }, texture, (Module*)gameAt);
	settingsButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControl(GuiControlType::BUTTON, "", { Engine::GetInstance().window->width - 20,50,10,10 }, texture, (Module*)gameAt);
	exitGameButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControl(GuiControlType::BUTTON, "", { Engine::GetInstance().window->width - 20,70,10,10 }, texture, (Module*)gameAt);
	exitAppButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControl(GuiControlType::BUTTON, "", { Engine::GetInstance().window->width - 20,90,10,10 }, texture, (Module*)gameAt);
}

PauseUI::~PauseUI()
{
	delete pauseButton;
	delete resumeButton;
	delete settingsButton;
	delete exitGameButton;
	delete exitAppButton;
}

void PauseUI::Update(float dt)
{
	if (isPaused)
	{
		pauseButton->Disable();
		resumeButton->Enable();
		settingsButton->Enable();
		exitGameButton->Enable();
		exitAppButton->Enable();
	}
	else
	{
		pauseButton->Enable();
		resumeButton->Disable();
		settingsButton->Disable();
		exitGameButton->Disable();
		exitAppButton->Disable();
	}
	pauseButton->Update(dt);
	resumeButton->Update(dt);
	settingsButton->Update(dt);
	exitGameButton->Update(dt);
	exitAppButton->Update(dt);

	Render();
}

void PauseUI::Render()
{
	if (isPaused)
	{
		SDL_Rect rect;
		Engine::GetInstance().render->LockLayer(Render::Layer6);
		Engine::GetInstance().render->DrawRectangle({ 32,Engine::GetInstance().window->height/2, Engine::GetInstance().window->width - 64,24}, 0, 0, 0, 255, true, false);
		Engine::GetInstance().render->DrawRectangle({ 32,Engine::GetInstance().window->height/2, Engine::GetInstance().window->width - 64,24}, 255, 255, 255, 255, false, false);
		rect = { 0,16,16,16 };
		Engine::GetInstance().render->DrawTexture(texture,-Engine::GetInstance().render->camera.x + 33, -Engine::GetInstance().render->camera.y +1+ Engine::GetInstance().window->height / 2,SDL_FLIP_NONE,&rect);
		rect = { 16,16,16,16 };
		Engine::GetInstance().render->DrawTexture(texture, -Engine::GetInstance().render->camera.x + Engine::GetInstance().window->width - 49,-Engine::GetInstance().render->camera.y +1+ Engine::GetInstance().window->height / 2,SDL_FLIP_NONE,&rect);
		rect = { 32,16,16,16 };
		Engine::GetInstance().render->DrawTexture(texture, -Engine::GetInstance().render->camera.x + Engine::GetInstance().window->width - 49, -Engine::GetInstance().render->camera.y + 7 + Engine::GetInstance().window->height / 2, SDL_FLIP_NONE, &rect);
		rect = { 48,16,16,16 };
		Engine::GetInstance().render->DrawTexture(texture, -Engine::GetInstance().render->camera.x + 33, -Engine::GetInstance().render->camera.y + 7 + Engine::GetInstance().window->height / 2, SDL_FLIP_NONE, &rect);

		Engine::GetInstance().render->UnlockLayer();
	}
	pauseButton->Render();
	resumeButton->Render();
	settingsButton->Render();
	exitGameButton->Render();
	exitAppButton->Render();
}

void PauseUI::CleanUp()
{
	pauseButton->CleanUp();
	resumeButton->CleanUp();
	settingsButton->CleanUp();
	exitGameButton->CleanUp();
	exitAppButton->CleanUp();
}

void PauseUI::SetPause(bool p)
{
	isPaused = p;
}

bool PauseUI::GetPause()
{
	return isPaused;
}
