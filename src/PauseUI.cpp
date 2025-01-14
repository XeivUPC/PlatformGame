#include "PauseUI.h"
#include "GuiManager.h"
#include "Engine.h"
#include "Window.h"
#include "Textures.h"
#include "GuiControlButton.h"


PauseUI::PauseUI(Module* ModuleAt): UI(ModuleAt)
{
	pauseButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ 250,0,16,16 }, texture, moduleAt);
	pauseButton->SetRectangle({ 0,0,16,16 }, GuiControlState::NORMAL);
	pauseButton->SetRectangle({ 16,0,16,16 }, GuiControlState::FOCUSED);
	pauseButton->SetRectangle({ 32,0,16,16 }, GuiControlState::PRESSED);
	pauseButton->SetRectangle({ 0,0,16,16 }, GuiControlState::DISABLED);
	resumeButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ 45,Engine::GetInstance().window->height / 2 + 6,53,11 }, texture, moduleAt);
	resumeButton->SetRectangle({ 0,16,53,11 }, GuiControlState::NORMAL);
	resumeButton->SetRectangle({ 53,16,53,11 }, GuiControlState::FOCUSED);
	resumeButton->SetRectangle({ 106,16,53,11 }, GuiControlState::PRESSED);
	settingsButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ 109,Engine::GetInstance().window->height / 2 + 6,69,11 }, texture, moduleAt);
	settingsButton->SetRectangle({ 0,27,69,11 }, GuiControlState::NORMAL);
	settingsButton->SetRectangle({ 69,27,69,11 }, GuiControlState::FOCUSED);
	settingsButton->SetRectangle({ 138,27,69,11 }, GuiControlState::PRESSED);
	exitGameButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ 189,Engine::GetInstance().window->height / 2 + 6,77,11 }, texture, moduleAt);
	exitGameButton->SetRectangle({ 0,38,77,11 }, GuiControlState::NORMAL);
	exitGameButton->SetRectangle({ 77,38,77,11 }, GuiControlState::FOCUSED);
	exitGameButton->SetRectangle({ 154,38,77,11 }, GuiControlState::PRESSED);
	exitAppButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ 277,Engine::GetInstance().window->height / 2 + 6,77,11 }, texture, moduleAt);
	exitAppButton->SetRectangle({ 0,49,77,11 }, GuiControlState::NORMAL);
	exitAppButton->SetRectangle({ 77,49,77,11 }, GuiControlState::FOCUSED);
	exitAppButton->SetRectangle({ 154,49,77,11 }, GuiControlState::PRESSED);

	resumeButton->Disable();
	settingsButton->Disable();
	exitGameButton->Disable();
	exitAppButton->Disable();
}

PauseUI::~PauseUI()
{
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
		Engine::GetInstance().render->DrawRectangle({ 0,0, Engine::GetInstance().window->width,Engine::GetInstance().window->height}, 0, 0, 0, 175, true, false);
		Engine::GetInstance().render->DrawRectangle({ 32,Engine::GetInstance().window->height/2, Engine::GetInstance().window->width - 64,24}, 0, 0, 0, 255, true, false);
		Engine::GetInstance().render->DrawRectangle({ 32,Engine::GetInstance().window->height/2, Engine::GetInstance().window->width - 64,24}, 255, 255, 255, 255, false, false);
		rect = { 0,64,16,16 };
		Engine::GetInstance().render->DrawTexture(texture,-Engine::GetInstance().render->camera.x + 33, -Engine::GetInstance().render->camera.y +1+ Engine::GetInstance().window->height / 2,SDL_FLIP_NONE,&rect);
		rect = { 16,64,16,16 };
		Engine::GetInstance().render->DrawTexture(texture, -Engine::GetInstance().render->camera.x + Engine::GetInstance().window->width - 49,-Engine::GetInstance().render->camera.y +1+ Engine::GetInstance().window->height / 2,SDL_FLIP_NONE,&rect);
		rect = { 32,64,16,16 };
		Engine::GetInstance().render->DrawTexture(texture, -Engine::GetInstance().render->camera.x + Engine::GetInstance().window->width - 49, -Engine::GetInstance().render->camera.y + 7 + Engine::GetInstance().window->height / 2, SDL_FLIP_NONE, &rect);
		rect = { 48,64,16,16 };
		Engine::GetInstance().render->DrawTexture(texture,  + 33, -Engine::GetInstance().render->camera.y + 7 + Engine::GetInstance().window->height / 2, SDL_FLIP_NONE, &rect);
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

	delete pauseButton;
	delete resumeButton;
	delete settingsButton;
	delete exitGameButton;
	delete exitAppButton;
}

void PauseUI::SetPause(bool p)
{
	isPaused = p;
}

bool PauseUI::GetPause()
{
	return isPaused;
}
