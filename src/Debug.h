#pragma once
#include "Module.h"
#include "Engine.h"
#include "SDL2/SDL.h"
#include <vector>

class Debug : public Module
{
private:
	std::vector<bool>debug;
public:
	Debug();
	virtual ~Debug();
	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool HasDebug(int option);
};