#include "TitleScene.h"
#include "GameScene.h"
#include "GuiManager.h"
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
	ui = new TitleUI(this);
	return true;
}

bool TitleScene::PreUpdate()
{
	return true;
}

bool TitleScene::Update(float dt)
{
	ui->Update(dt);
	/*if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		Engine::GetInstance().game_scene->Enable();
		Disable();
	}*/
	return true;
}

bool TitleScene::PostUpdate()
{
	return true;
}

bool TitleScene::CleanUp()
{
	LOG("Freeing TitleScreen");
	ui->CleanUp();
	delete ui;
	return true;
}

bool TitleScene::LoadParameters(xml_node parameters)
{
	return true;
}

bool TitleScene::OnGuiMouseClickEvent(GuiControl* control)
{
	return true;
}