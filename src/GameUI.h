#pragma once
#include "SDL2/SDL.h"
#include "UI.h"
class Counter;
class Module;
class Player;
class GameUI : public UI
{
private:
	Player* player;
public:
	GameUI(Module* ModuleAt, Player* p);
	~GameUI();
	void Update(float dt);
	void Render();
	void CleanUp();
};