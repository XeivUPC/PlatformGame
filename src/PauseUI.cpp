#include "PauseUI.h"
#include "GuiManager.h"
#include "Engine.h"
#include "Window.h"
#include "Textures.h"
#include "GuiControlButton.h"


PauseUI::PauseUI(GameScene* GameAt)
{
	gameAt = GameAt;
	texture = Engine::GetInstance().textures->Load("Assets/Textures/UI1.png");
	pauseButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControl(GuiControlType::BUTTON, "", { Engine::GetInstance().window->width - 20,10,10,10 }, texture, (Module*)gameAt);
	pauseButton->SetRectangle({ 0,0,16,16 }, GuiControlState::NORMAL);
	pauseButton->SetRectangle({ 16,0,16,16 }, GuiControlState::FOCUSED);
	pauseButton->SetRectangle({ 32,0,16,16 }, GuiControlState::PRESSED);
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
	pauseButton->Update(dt);
	resumeButton->Update(dt);
	settingsButton->Update(dt);
	exitGameButton->Update(dt);
	exitAppButton->Update(dt);

	Render();
}

void PauseUI::Render()
{
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
