#include "Debug.h"
#include "Input.h"
#include "Scene.h"
#include "Box2DRender.h"

Debug::Debug()
{
	for (int i = 0; i < 4; i++)
	{
		debug.push_back(false);
	}
}

Debug::~Debug()
{
	
}

bool Debug::Awake()
{
	return true;
}

bool Debug::Start()
{
	return true;
}

bool Debug::PreUpdate()
{
	return true;
}

bool Debug::Update(float dt)
{
	//H Shows / hides a help menu showing the keys
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		debug[0] = !debug[0];
	//F1 / F2 Start from the first / second level
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN){}

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN){}

	//F3 Start from the beginning of the current level
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN){}

	//F5 Save the current game state
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN){}

	//F6 Load the previous state(even across levels)
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN){}

	//F9 View colliders / logic / paths
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		debug[1] = !debug[1];
	}
	//F10 God Mode(fly around, cannot be killed)
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		debug[2] = !debug[2];
	//F11 Enable / Disable FPS cap to 30
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		debug[3] = !debug[3];
	return true;
}

bool Debug::PostUpdate()
{
	return true;
}

bool Debug::CleanUp()
{
	debug.clear();
	return true;
}

bool Debug::HasDebug(int option)
{
	return debug[option];
}
