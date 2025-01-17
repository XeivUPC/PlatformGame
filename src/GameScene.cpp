#include "GameScene.h"
#include "TitleScene.h"
#include "Render.h"
#include "Log.h"
#include "Engine.h"
#include "Audio.h"
#include "EntityManager.h"
#include "GuiControlToggle.h"
#include "GuiControlSlider.h"
#include "LevelManager.h"
#include "Window.h"
#include "GameUI.h"
#include "Physics.h"
#include "PauseUI.h"
#include "SettingsUI.h"
#include "FadeUI.h"

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
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER, true);
	Engine::GetInstance().levelManager->LoadLevel(0);

	exitGame = false;
	goToMainMenu = false;
	pause = new PauseUI(this);
	settings = new SettingsUI(this);
	fade = new FadeUI(this);
	ui = new GameUI(this, player);
	settings->Init();
	fade->StartFadeOut({ 0,0,0,255 }, 1000.0f);
	return true;
}

bool GameScene::PreUpdate()
{
	return true;
}

bool GameScene::Update(float dt)
{
	ui->Update(dt);
	if (!fade->IsFading()){
		pause->Update(dt);
	}
	settings->Update(dt);
	fade->Update(dt);
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
	LOG("Freeing GameScene");
	ui->CleanUp();
	pause->CleanUp();
	settings->CleanUp();
	fade->CleanUp();
	delete ui;
	delete pause;
	delete settings;
	delete fade;

	Engine::GetInstance().entityManager->paused = false;
	Engine::GetInstance().physics->StartSimulation();

	Engine::GetInstance().audio->StopMusic();
	Engine::GetInstance().render->camera.x = 0;
	Engine::GetInstance().render->camera.y = 0;
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
		Engine::GetInstance().entityManager->paused = true;
		Engine::GetInstance().physics->PauseSimulation();
		pause->SetPause(true);
	}
	else if (control == (GuiControl*)pause->resumeButton)
	{
		pause->SetPause(false);
		Engine::GetInstance().entityManager->paused = false;
		Engine::GetInstance().physics->StartSimulation();
	}

	else if (control == (GuiControl*)pause->settingsButton)
	{
		// Go Settings
		settings->SetIfOpen(true);
		pause->SetInteractable(false);
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
	else if (control == (GuiControl*)settings->backButton) {
		settings->SetIfOpen(false);
		pause->SetInteractable(true);
	}
	else if (control == (GuiControl*)settings->videoButton)
	{
		settings->OpenVideoOptions();
	}
	else if (control == (GuiControl*)settings->audioButton)
	{
		settings->OpenAudioOptions();
	}
	else if (control == (GuiControl*)settings->fullscreenToggle)
	{
		Engine::GetInstance().window->SetFullScreen(settings->fullscreenToggle->IsOn());
	}
	else if (control == (GuiControl*)settings->generalVolumeSlider)
	{
		Engine::GetInstance().audio->SetMasterVolume(settings->generalVolumeSlider->GetValue());
	}
	else if (control == (GuiControl*)settings->musicVolumeSlider)
	{
		Engine::GetInstance().audio->SetMusicVolume(settings->musicVolumeSlider->GetValue());
	}
	else if (control == (GuiControl*)settings->sfxVolumeSlider)
	{
		Engine::GetInstance().audio->SetFxVolume(settings->sfxVolumeSlider->GetValue());
	}
	return true;
}
