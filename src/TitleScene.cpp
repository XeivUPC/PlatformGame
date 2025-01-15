#include "TitleScene.h"
#include "GameScene.h"
#include "GuiManager.h"
#include "LevelManager.h"
#include "Window.h"
#include "Input.h"
#include "Engine.h"
#include "Audio.h"
#include "Log.h"
#include "SettingsUI.h"
#include "CreditsUI.h"
#include "GuiControlToggle.h"
#include "GuiControlSlider.h"

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
	settings = new SettingsUI(this);
	credits = new CreditsUI(this);
	ui = new TitleUI(this);
	goToGame = false;
	exitGame = false;
	loadSaveFile = false;
	settings->Init();


	CheckIfSaveFileExists("entitiesSaveData.xml");

	return true;
}

bool TitleScene::PreUpdate()
{
	return true;
}

bool TitleScene::Update(float dt)
{
	ui->Update(dt);
	settings->Update(dt);
	credits->Update(dt);
	return true;
}

bool TitleScene::PostUpdate()
{
	if (goToGame) {
		Engine::GetInstance().game_scene->Enable();
		if(loadSaveFile)
			Engine::GetInstance().levelManager->LoadSaveFile("entitiesSaveData.xml");
		Disable();
	}

	return !exitGame;
}

bool TitleScene::CleanUp()
{
	LOG("Freeing TitleScreen");
	ui->CleanUp();
	settings->CleanUp();
	credits->CleanUp();
	delete ui;
	delete settings;
	delete credits;
	return true;
}

bool TitleScene::LoadParameters(xml_node parameters)
{
	return true;
}

bool TitleScene::OnGuiMouseClickEvent(GuiControl* control)
{
	if (control == (GuiControl*)ui->playButton)
	{
		goToGame = true;
	}
	else if (control == (GuiControl*)ui->continueButton)
	{
		//Load Game with SaveFile
		goToGame = true;
		loadSaveFile = true;
	}
	else if (control == (GuiControl*)ui->settingsButton)
	{
		settings->SetIfOpen(true);
		ui->SetInteractable(false);
		// Go Settings
	}
	else if (control == (GuiControl*)ui->creditsButton)
	{
		credits->SetIfOpen(true);
		ui->SetInteractable(false);

	}
	else if (control == (GuiControl*)ui->exitButton)
	{
		exitGame = true;
	}
	
	else if (control == (GuiControl*)credits->backButton)
	{
		credits->SetIfOpen(false);
		ui->SetInteractable(true);
	}
	else if (control == (GuiControl*)settings->backButton) {
		settings->SetIfOpen(false);
		ui->SetInteractable(true);
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

void TitleScene::CheckIfSaveFileExists(std::string path)
{
	path = "Assets/SaveData/" + path;
	xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file(path.c_str());
	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", path.c_str(), result.description());
	}
	else {
		int level = saveFile.child("entities").child("player").child("level").attribute("value").as_int();
		if (level == -1) {
			ui->continueButton->Disable();
		}
	}
}
