#include "TitleUI.h"
#include "Engine.h"
#include "GuiControlButton.h"
#include "Textures.h"
#include "GuiManager.h"
TitleUI::TitleUI(Module* ModuleAt) : UI(ModuleAt)
{
	playButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ 428/2-85/2,142,85,11 }, texture, moduleAt);
	playButton->SetRectangle({ 0,96,85,11 }, GuiControlState::NORMAL);
	playButton->SetRectangle({ 0,96,85,11 }, GuiControlState::FOCUSED);
	playButton->SetRectangle({ 0,96,85,11 }, GuiControlState::PRESSED);
	continueButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ 428 / 2 - 69/2,153,69,11 }, texture, moduleAt);
	continueButton->SetRectangle({ 0,107,69,11 }, GuiControlState::NORMAL);
	continueButton->SetRectangle({ 0,107,69,11 }, GuiControlState::FOCUSED);
	continueButton->SetRectangle({ 0,107,69,11 }, GuiControlState::PRESSED);
	settingsButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ 428 / 2 - 69 / 2,164,69,11 }, texture, moduleAt);
	settingsButton->SetRectangle({ 0,118,69,11 }, GuiControlState::NORMAL);
	settingsButton->SetRectangle({ 0,118,69,11 }, GuiControlState::FOCUSED);
	settingsButton->SetRectangle({ 0,118,69,11 }, GuiControlState::PRESSED);
	creditsButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ 428 / 2 - 58 / 2,175,58,11 }, texture, moduleAt);
	creditsButton->SetRectangle({ 0,129,58,11 }, GuiControlState::NORMAL);
	creditsButton->SetRectangle({ 0,129,58,11 }, GuiControlState::FOCUSED);
	creditsButton->SetRectangle({ 0,129,58,11 }, GuiControlState::PRESSED);
	exitButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ 428 / 2 - 37 / 2,186,37,11 }, texture, moduleAt);
	exitButton->SetRectangle({ 0,140,37,11 }, GuiControlState::NORMAL);
	exitButton->SetRectangle({ 0,140,37,11 }, GuiControlState::FOCUSED);
	exitButton->SetRectangle({ 0,140,37,11 }, GuiControlState::PRESSED);
}

TitleUI::~TitleUI()
{
}

void TitleUI::Update(float dt)
{
	playButton->Update(dt);
	continueButton->Update(dt);
	settingsButton->Update(dt);
	creditsButton->Update(dt);
	exitButton->Update(dt);

	if (playButton->CurrentState() == GuiControlState::FOCUSED)shovel_y = playButton->bounds.y;
	else if (continueButton->CurrentState() == GuiControlState::FOCUSED)shovel_y = continueButton->bounds.y;
	else if (settingsButton->CurrentState() == GuiControlState::FOCUSED)shovel_y = settingsButton->bounds.y;
	else if (creditsButton->CurrentState() == GuiControlState::FOCUSED)shovel_y = creditsButton->bounds.y;
	else if (exitButton->CurrentState() == GuiControlState::FOCUSED)shovel_y = exitButton->bounds.y;
	else shovel_y = -10;

	Render();
}

void TitleUI::Render()
{
	Engine::GetInstance().render->LockLayer(Render::Layer6);
	SDL_Rect rect = { 0,0,427,239 };
	Engine::GetInstance().render->DrawTexture(Engine::GetInstance().textures->GetTexture("MainMenu"), 0,0, SDL_FLIP_NONE, &rect);
	playButton->Render();
	continueButton->Render();
	settingsButton->Render();
	creditsButton->Render();
	exitButton->Render();
	rect = { 112,80,32,8 };
	Engine::GetInstance().render->LockLayer(Render::Layer6);
	Engine::GetInstance().render->DrawTexture(texture, 133, shovel_y, SDL_FLIP_NONE, &rect);
	Engine::GetInstance().render->UnlockLayer();
}

void TitleUI::CleanUp()
{
	playButton->CleanUp();
	continueButton->CleanUp();
	settingsButton->CleanUp();
	creditsButton->CleanUp();
	exitButton->CleanUp();

	delete playButton;
	delete continueButton;
	delete settingsButton;
	delete creditsButton;
	delete exitButton;
}