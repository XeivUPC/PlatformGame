#pragma once
#include "Module.h"

class AssetLoader : public Module
{
public:

	AssetLoader();

	// Destructor
	virtual ~AssetLoader();

	bool Start();

	// Called before quitting
	bool CleanUp();
};

