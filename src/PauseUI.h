#pragma once
#include "SDL2/SDL.h"
class GuiControlButton;
class GameScene;
class PauseUI
{
private:
	GameScene* gameAt;
	SDL_Texture* texture;
public:
	PauseUI(GameScene* GameAt);
	~PauseUI();
	void Update(float dt);
	void Render();
	void CleanUp();
	GuiControlButton* pauseButton;
	GuiControlButton* resumeButton;
	GuiControlButton* settingsButton;
	GuiControlButton* exitGameButton;
	GuiControlButton* exitAppButton;
};