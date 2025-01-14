#include "GuiManager.h"
#include "Engine.h"
#include "Textures.h"

#include "GuiControlButton.h"
#include "GuiControlToggle.h"
#include "GuiControlSlider.h"
#include "Audio.h"

GuiManager::GuiManager() :Module()
{
	name = "guiManager";
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}

// L16: TODO 1: Implement CreateGuiControl function that instantiates a new GUI control and add it to the list of controls
GuiControl* GuiManager::CreateGuiControlButton(SDL_Rect bounds, SDL_Texture* texture, Module* observer)
{
	GuiControl* guiControl = new GuiControlButton(bounds, texture);
	guiControl->SetObserver(observer);
	return guiControl;
}

GuiControl* GuiManager::CreateGuiControlToggle(SDL_Rect bounds, SDL_Texture* texture, bool status, Module* observer)
{
	GuiControl* guiControl = new GuiControlToggle(bounds, texture, status);
	guiControl->SetObserver(observer);
	return guiControl;
}

GuiControl* GuiManager::CreateGuiControlSlider(SDL_Rect bounds, SDL_Texture* texture, GuiControlButton* t, float v, float mn, float mx, Module* observer)
{
	GuiControl* guiControl = new GuiControlSlider(bounds, t, texture, v, mn, mx);
	guiControl->SetObserver(observer);
	return guiControl;
}