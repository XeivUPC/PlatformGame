#include "Debug.h"
#include "Input.h"
#include "Window.h"
#include "TextGenerator.h"
#include "Box2DRender.h"

Debug::Debug()
{
	for (int i = 0; i < 5; i++)
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

	//F8 Full Screen
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		debug[4] = !debug[4];
		Engine::GetInstance().window->SetFullScreen(debug[4]);
	}

	//F9 View colliders / logic / paths
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		debug[1] = !debug[1];
	}
	//F10 God Mode(fly around, cannot be killed)
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		debug[2] = !debug[2];
	//F11 Enable / Disable FPS cap to 30
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		debug[3] = !debug[3];
		Engine::GetInstance().SetMaxFrameDuration(debug[3] ? 32 : 16);

	}
	DrawDebugMenu();
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

void Debug::DrawDebugMenu()
{
	if (!HasDebug(0))return;
	
	Engine::GetInstance().render->LockLayer(Render::RenderLayers::Layer7);



	Engine::GetInstance().render->DrawRectangle({ -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y, 128,Engine::GetInstance().render->camera.h},0,0,0, (int)(255*0.5f), true);
	Engine::GetInstance().text->Write("DEBUG MENU", -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y + 16*2);
	Engine::GetInstance().text->Write("(F1) prev level", -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y + 16 * 3);
	Engine::GetInstance().text->Write("(F2) next level", -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y + 16 *4);
	Engine::GetInstance().text->Write("(F3) restart", -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y + 16 * 5);
	Engine::GetInstance().text->Write("(F5) save", -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y + 16 * 6);
	Engine::GetInstance().text->Write("(F6) load", -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y + 16 * 7);
	Engine::GetInstance().text->Write("(F7) navigate", -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y + 16 * 8);

	if (HasDebug(4))
		Engine::GetInstance().text->Write("(F8) full screen", -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y + 16 * 9, { 0,255,0,255 });
	else
		Engine::GetInstance().text->Write("(F8) full screen", -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y + 16 * 9);
	if(HasDebug(1))
		Engine::GetInstance().text->Write("(F9) colliders", -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y + 16*10, { 0,255,0,255 });
	else
		Engine::GetInstance().text->Write("(F9) colliders", -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y + 16* 10);
	if (HasDebug(2))
		Engine::GetInstance().text->Write("(F10) GOD mode", -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y + 16 * 11, { 0,255,0,255 });
	else
		Engine::GetInstance().text->Write("(F10) GOD mode", -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y + 16 * 11);
	if (HasDebug(3))
		Engine::GetInstance().text->Write("(F11) FPS CAP 30", -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y + 16 * 12, { 0,255,0,255 });
	else
		Engine::GetInstance().text->Write("(F11) FPS CAP 30", -Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y + 16 * 12);

	Engine::GetInstance().render->UnlockLayer();
}

bool Debug::HasDebug(int option)
{
	return debug[option];
}
