#pragma once
#include "SDL2/SDL.h"
#include "UI.h"
class GuiControlButton;
class Module;
class PauseUI : public UI
{
private:
	bool isPaused = false;
	bool isInteractable = true;
public:
	PauseUI(Module* ModuleAt);
	~PauseUI();
	void Update(float dt);
	void Render();
	void CleanUp();
	void SetPause(bool p);
	bool GetPause();
	GuiControlButton* pauseButton;
	GuiControlButton* resumeButton;
	GuiControlButton* settingsButton;
	GuiControlButton* exitGameButton;
	GuiControlButton* exitAppButton;
};