#include "GameScene.h"
#include "Log.h"
#include "Engine.h"
#include "EntityManager.h"
#include "LevelManager.h"
#include "PauseUI.h"

GameScene::GameScene(bool isActive) : Module(isActive)
{
	name = "GameScene";
}

GameScene::~GameScene()
{
}

bool GameScene::Awake()
{
	LOG("Loading GameScene");
	bool ret = true;
	

	return ret;
}

bool GameScene::Start()
{
	pause = new PauseUI(this);
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER, true);
	Engine::GetInstance().levelManager->LoadLevel(0);
	return true;
}

bool GameScene::PreUpdate()
{
	return true;
}

bool GameScene::Update(float dt)
{

	pause->Update(dt);
	return true;
}

bool GameScene::PostUpdate()
{
	bool ret = true;
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	return ret;
}

bool GameScene::CleanUp()
{
	LOG("Freeing GameScene");
	return true;
}

bool GameScene::LoadParameters(xml_node parameters)
{

	return true;
}

bool GameScene::OnGuiMouseClickEvent(GuiControl* control)
{
	if (control == (GuiControl*)pause->pauseButton)
	{
		//pause
		pause->SetPause(true);
	}
	return true;
}
