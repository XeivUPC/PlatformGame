#pragma once
#include "SDL2/SDL.h"
class GuiControlButton;
class Scene;
class PauseUI
{
private:
	Scene* gameAt;
	SDL_Texture* texture;
	bool isPaused = false;
public:
	PauseUI(Scene* GameAt);
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