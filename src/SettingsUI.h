#pragma once
#include "SDL2/SDL.h"
#include "UI.h"

class GuiControlButton;
class GuiControlSlider;
class GuiControlToggle;
class Module;
class SettingsUI : public UI
{
private:
	bool isOpen = false;
	bool audioSettings = false;
	bool videoSettigns = false;

	SDL_Rect settingsSlotsAnchor;
public:
	SettingsUI(Module* ModuleAt);
	~SettingsUI();
	void Init();
	void Update(float dt);
	void Render();
	void CleanUp();

	void SetIfOpen(bool p);
	bool IsOpen();

	void OpenAudioOptions();
	void OpenVideoOptions();

	GuiControlButton* backButton;
	GuiControlButton* audioButton;
	GuiControlButton* videoButton;


	GuiControlToggle* fullscreenToggle;

	GuiControlSlider* generalVolumeSlider;
	GuiControlSlider* musicVolumeSlider;
	GuiControlSlider* sfxVolumeSlider;

};