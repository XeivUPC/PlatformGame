#include "TitleScene.h"
#include "GameScene.h"
#include "GuiManager.h"
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
	settings->Init();
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
	/*if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		Engine::GetInstance().game_scene->Enable();
		Disable();
	}*/
	return true;
}

bool TitleScene::PostUpdate()
{
	if (goToGame) {
		Engine::GetInstance().game_scene->Enable();
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