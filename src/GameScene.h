#pragma once
#include "Module.h"
#include "Player.h"
#include <box2d/box2d.h>


class PauseUI;
class GameUI;
class GameScene : public Module
{
public:

	GameScene(bool isActive = true);

	// Destructor
	virtual ~GameScene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load Parameters from config file
	bool LoadParameters(xml_node parameters);

	bool OnGuiMouseClickEvent(GuiControl* control);

	Player* player;

private:
	PauseUI* pause = nullptr;
	GameUI* ui = nullptr;
	bool exitGame = false;
	bool goToMainMenu = false;
};

