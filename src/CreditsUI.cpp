#include "CreditsUI.h"
#include "Textures.h"
#include "Engine.h"
#include "Window.h"
#include "Render.h"
#include "GuiManager.h"
#include "GuiControlButton.h"

CreditsUI::CreditsUI(Module* ModuleAt) : UI(ModuleAt)
{
	bg_texture = Engine::GetInstance().textures->GetTexture("Credits");

	backButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ 37/2,13/2, 37, 13 }, texture, moduleAt);
	backButton->SetRectangle({ 0,151,37,13 }, GuiControlState::NORMAL);
	backButton->SetRectangle({ 37,151,37,13 }, GuiControlState::FOCUSED);
	backButton->SetRectangle({ 37 * 2,151,37,13 }, GuiControlState::PRESSED);

}

CreditsUI::~CreditsUI()
{
}

void CreditsUI::Update(float dt)
{
	if (IsOpen()) {
		if(isInteractable)
			backButton->Update(dt);
		Render();
	}
}

void CreditsUI::Render()
{
	Engine::GetInstance().render->LockLayer(Render::Layer6);
	SDL_Rect rect = { 0,0,427,239 };
	Engine::GetInstance().render->DrawTexture(bg_texture, 0, 0, SDL_FLIP_NONE, &rect);
	Engine::GetInstance().render->UnlockLayer();

	backButton->Render();
}

void CreditsUI::CleanUp()
{
	backButton->CleanUp();
	delete backButton;
}

void CreditsUI::SetIfOpen(bool p)
{
	isOpen = p;
}

bool CreditsUI::IsOpen()
{
	return isOpen;
}
