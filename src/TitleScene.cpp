#include "TitleScene.h"
#include "GameScene.h"
#include "Input.h"
#include "Engine.h"
#include "Log.h"

TitleScene::TitleScene(bool isActive) : Module(isActive)
{
	name = "TitleScreen";
}

TitleScene::~TitleScene()
{
}

bool TitleScene::Awake()
{
	LOG("Loading TitleScreen");
	return true;
}

bool TitleScene::Start()
{
	return true;
}

bool TitleScene::PreUpdate()
{
	return true;
}

bool TitleScene::Update(float dt)
{
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		Engine::GetInstance().game_scene->Enable();
		Disable();
	}
	return true;
}

bool TitleScene::PostUpdate()
{
	
	return true;
}

bool TitleScene::CleanUp()
{
	LOG("Freeing TitleScreen");
	return true;
}

bool TitleScene::LoadParameters(xml_node parameters)
{
	return true;
}
