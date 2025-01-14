#include "GameScene.h"
#include "TitleScene.h"
#include "Log.h"
#include "Engine.h"
#include "EntityManager.h"
#include "LevelManager.h"
#include "GameUI.h"
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
	ui = new GameUI(this, player);
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER, true);
	Engine::GetInstance().levelManager->LoadLevel(0);

	exitGame = false;
	goToMainMenu = false;
	return true;
}

bool GameScene::PreUpdate()
{
	return true;
}

bool GameScene::Update(float dt)
{
	ui->Update(dt);
	pause->Update(dt);
	return true;
}

bool GameScene::PostUpdate()
{
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		exitGame = true;

	if (goToMainMenu) {
		pause->SetPause(false);
		Engine::GetInstance().title_scene->Enable();
		Disable();
	}
	return !exitGame;
}

bool GameScene::CleanUp()
{
	/*ui->CleanUp();
	pause->CleanUp();
	delete ui;
	delete pause;*/
	LOG("Freeing GameScene");
	pause->CleanUp();
	delete pause;
	Engine::GetInstance().entityManager->CleanUp();
	Engine::GetInstance().levelManager->CleanUp();


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
		pause->SetPause(true);
	}
	else if (control == (GuiControl*)pause->resumeButton)
	{
		pause->SetPause(false);
	}

	else if (control == (GuiControl*)pause->settingsButton)
	{
		// Go Settings
	}

	else if (control == (GuiControl*)pause->exitGameButton)
	{
		//Go Main Menu
		goToMainMenu = true;
		
	}
	else if (control == (GuiControl*)pause->exitAppButton)
	{
		//Quit Game
		exitGame = true;
	}
	return true;
}
