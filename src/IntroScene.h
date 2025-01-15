#pragma once

#include "Module.h"
#include <box2d/box2d.h>
#include "IntroUI.h"
#include "Timer.h"

class IntroScene : public Module
{
public:

	IntroScene(bool isActive = true);

	// Destructor
	virtual ~IntroScene();

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

	IntroUI* ui = nullptr;

private:
	int timeInIntro = 3000;
	Timer timer;

};

