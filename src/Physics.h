#pragma once
#include "Module.h"
#include <box2d/box2d.h>


class Physics : public Module
{
public:

	Physics();

	// Destructor
	virtual ~Physics();

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

public:
	b2World* world;
};
