#pragma once
#include "Module.h"
#include <box2d/box2d.h>
#include "TitleUI.h"
#include <string>

class SettingsUI;
class CreditsUI;

class TitleScene : public Module
{
public:

	TitleScene(bool isActive = true);

	// Destructor
	virtual ~TitleScene();

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
	TitleUI* ui = nullptr;
	bool LoadParameters(xml_node parameters);
	bool OnGuiMouseClickEvent(GuiControl* control);

private:

	void CheckIfSaveFileExists(std::string path);
	SettingsUI* settings = nullptr;
	CreditsUI* credits = nullptr;

	bool goToGame = false;
	bool loadSaveFile = false;
	bool exitGame = false;
};

