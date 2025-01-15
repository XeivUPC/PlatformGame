#include "SettingsUI.h"
#include "GuiManager.h"
#include "TextGenerator.h"
#include "Engine.h"
#include "Window.h"
#include "Textures.h"
#include "Audio.h"
#include "GuiControlButton.h"
#include "GuiControlToggle.h"
#include "GuiControlSlider.h"

SettingsUI::SettingsUI(Module* ModuleAt) : UI(ModuleAt)
{

	settingsSlotsAnchor = { 72 + 134,Engine::GetInstance().window->height / 2 - 80 + 26 , 100, 116};

	Vector2D anchor = {72+5,32+26};
	backButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ (int)anchor.getX() + 0,(int)anchor.getY() + 0,37,13 }, texture, moduleAt);
	backButton->SetRectangle({ 0,151,37,13 }, GuiControlState::NORMAL);
	backButton->SetRectangle({ 37,151,37,13 }, GuiControlState::FOCUSED);
	backButton->SetRectangle({ 37*2,151,37,13 }, GuiControlState::PRESSED);

	audioButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ (int)anchor.getX() + 0,(int)anchor.getY() + 38/2,45,13 }, texture, moduleAt);
	audioButton->SetRectangle({ 0,164,45,13 }, GuiControlState::NORMAL);
	audioButton->SetRectangle({ 45,164,45,13 }, GuiControlState::FOCUSED);
	audioButton->SetRectangle({ 45 * 2,164,45,13 }, GuiControlState::PRESSED);

	videoButton = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ (int)anchor.getX() + 0,(int)anchor.getY() + 76/2,45,13 }, texture, moduleAt);
	videoButton->SetRectangle({ 0,177,45,13 }, GuiControlState::NORMAL);
	videoButton->SetRectangle({ 45,177,45,13 }, GuiControlState::FOCUSED);
	videoButton->SetRectangle({ 45 * 2,177,45,13 }, GuiControlState::PRESSED);

	fullscreenToggle = (GuiControlToggle*)Engine::GetInstance().ui->CreateGuiControlToggle({ settingsSlotsAnchor.x + settingsSlotsAnchor.w/2 -8, settingsSlotsAnchor.y + settingsSlotsAnchor.h / 2 -8,16,16 }, texture, false, moduleAt);
	fullscreenToggle->SetRectangle({ 64,64,16,16 }, GuiControlState::NORMAL);
	fullscreenToggle->SetRectangle({ 64+16,64,16,16 }, GuiControlState::FOCUSED);
	fullscreenToggle->SetRectangle({ 64+32,64,16,16 }, GuiControlState::PRESSED);
	




	GuiControlButton* thumb = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ (int)anchor.getX() + 0,(int)anchor.getY() + 76 / 2,6,12 }, texture, moduleAt);
	thumb->SetRectangle({ 192,66,6,12 }, GuiControlState::NORMAL);
	thumb->SetRectangle({ 192,66,6,12 }, GuiControlState::FOCUSED);
	thumb->SetRectangle({ 192,66,6,12 }, GuiControlState::PRESSED);
	generalVolumeSlider = (GuiControlSlider*)Engine::GetInstance().ui->CreateGuiControlSlider({ settingsSlotsAnchor.x + settingsSlotsAnchor.w / 2 - 40, settingsSlotsAnchor.y + settingsSlotsAnchor.h / 2 - 3 - 30,80,6 }, texture, thumb,1,0,1, moduleAt);
	generalVolumeSlider->SetRectangle({ 112,69,80,6 }, GuiControlState::NORMAL);


	thumb = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ (int)anchor.getX() + 0,(int)anchor.getY() + 76 / 2,6,12 }, texture, moduleAt);
	thumb->SetRectangle({ 192,66,6,12 }, GuiControlState::NORMAL);
	thumb->SetRectangle({ 192,66,6,12 }, GuiControlState::FOCUSED);
	thumb->SetRectangle({ 192,66,6,12 }, GuiControlState::PRESSED);
	musicVolumeSlider = (GuiControlSlider*)Engine::GetInstance().ui->CreateGuiControlSlider({ settingsSlotsAnchor.x + settingsSlotsAnchor.w / 2 - 40, settingsSlotsAnchor.y + settingsSlotsAnchor.h / 2 - 3 + 0,80,6 }, texture, thumb, 1, 0, 1, moduleAt);
	musicVolumeSlider->SetRectangle({ 112,69,80,6 }, GuiControlState::NORMAL);

	thumb = (GuiControlButton*)Engine::GetInstance().ui->CreateGuiControlButton({ (int)anchor.getX() + 0,(int)anchor.getY() + 76 / 2,6,12 }, texture, moduleAt);
	thumb->SetRectangle({ 192,66,6,12 }, GuiControlState::NORMAL);
	thumb->SetRectangle({ 192,66,6,12 }, GuiControlState::FOCUSED);
	thumb->SetRectangle({ 192,66,6,12 }, GuiControlState::PRESSED);
	sfxVolumeSlider = (GuiControlSlider*)Engine::GetInstance().ui->CreateGuiControlSlider({ settingsSlotsAnchor.x + settingsSlotsAnchor.w / 2 - 40, settingsSlotsAnchor.y + settingsSlotsAnchor.h / 2 - 3 +30,80,6 }, texture, thumb, 1, 0, 1, moduleAt);
	sfxVolumeSlider->SetRectangle({ 112,69,80,6 }, GuiControlState::NORMAL);
}

SettingsUI::~SettingsUI()
{
}

void SettingsUI::Init()
{
	sfxVolumeSlider->SetValue(Engine::GetInstance().audio->GetFxVolume());
	musicVolumeSlider->SetValue(Engine::GetInstance().audio->GetMusicVolume());
	generalVolumeSlider->SetValue(Engine::GetInstance().audio->GetGeneralVolume());

	if (Engine::GetInstance().window->IsFullScreen())
		fullscreenToggle->TurnOn();
}


void SettingsUI::Update(float dt)
{
	if (IsOpen()) {
		backButton->Update(dt);
		audioButton->Update(dt);
		videoButton->Update(dt);

		if (videoSettigns) {
			fullscreenToggle->Update(dt);
		}
		if (audioSettings) {
			generalVolumeSlider->Update(dt);
			musicVolumeSlider->Update(dt);
			sfxVolumeSlider->Update(dt);
		}
		Render();
	}	
}

void SettingsUI::Render()
{
	SDL_Rect rect;
	Engine::GetInstance().render->LockLayer(Render::Layer6);

	Engine::GetInstance().render->DrawRectangle({ 0,0, Engine::GetInstance().window->width,Engine::GetInstance().window->height }, 0, 0, 0, 120, true, false);

	Engine::GetInstance().render->DrawRectangle({ 72,Engine::GetInstance().window->height / 2 - 80, Engine::GetInstance().window->width - 144,160 }, 0, 0, 0, 255, true, false);
	Engine::GetInstance().render->DrawRectangle({ 72,Engine::GetInstance().window->height / 2 - 80, Engine::GetInstance().window->width - 144,160 }, 255, 255, 255, 255, false, false);

	rect = { 0,64,16,16 };
	Engine::GetInstance().render->DrawTexture(texture, -Engine::GetInstance().render->camera.x + 73, -Engine::GetInstance().render->camera.y + 1 + Engine::GetInstance().window->height / 2 - 80, SDL_FLIP_NONE, &rect);

	rect = { 16,64,16,16 };
	Engine::GetInstance().render->DrawTexture(texture, -Engine::GetInstance().render->camera.x + Engine::GetInstance().window->width - 49 - 40, -Engine::GetInstance().render->camera.y + 1 + Engine::GetInstance().window->height / 2 - 80, SDL_FLIP_NONE, &rect);

	rect = { 32,64,16,16 };
	Engine::GetInstance().render->DrawTexture(texture, -Engine::GetInstance().render->camera.x + Engine::GetInstance().window->width - 49 - 40, -Engine::GetInstance().render->camera.y + 7 + Engine::GetInstance().window->height / 2 + 56, SDL_FLIP_NONE, &rect);

	rect = { 48,64,16,16 };
	Engine::GetInstance().render->DrawTexture(texture, +73 - Engine::GetInstance().render->camera.x, -Engine::GetInstance().render->camera.y + 7 + Engine::GetInstance().window->height / 2 + 56, SDL_FLIP_NONE, &rect);


	Engine::GetInstance().text->Write("-options-", Engine::GetInstance().window->width/2 -Engine::GetInstance().render->camera.x - 40, -Engine::GetInstance().render->camera.y + 41, { 58,191,237,255 });


	if (videoSettigns) {
		Engine::GetInstance().render->DrawRectangle(settingsSlotsAnchor, 0, 0, 0, 255, true, false);
		Engine::GetInstance().render->DrawRectangle(settingsSlotsAnchor, 255, 255, 255, 255, false, false);

		Engine::GetInstance().text->Write("Fullscreen", fullscreenToggle->bounds.x - 8*4 - Engine::GetInstance().render->camera.x, fullscreenToggle->bounds.y - 20 - Engine::GetInstance().render->camera.y, { 255,255,255,255 });
		fullscreenToggle->Render();
	}
	else if (audioSettings) {
		Engine::GetInstance().render->DrawRectangle(settingsSlotsAnchor, 0, 0, 0, 255, true, false);
		Engine::GetInstance().render->DrawRectangle(settingsSlotsAnchor, 255, 255, 255, 255, false, false);

		Engine::GetInstance().text->Write("General", generalVolumeSlider->bounds.x - Engine::GetInstance().render->camera.x, generalVolumeSlider->bounds.y - 15 - Engine::GetInstance().render->camera.y, { 255,255,255,255 });
		Engine::GetInstance().text->Write("Music", musicVolumeSlider->bounds.x - Engine::GetInstance().render->camera.x, musicVolumeSlider->bounds.y - 15 - Engine::GetInstance().render->camera.y, { 255,255,255,255 });
		Engine::GetInstance().text->Write("Sfx", sfxVolumeSlider->bounds.x - Engine::GetInstance().render->camera.x, sfxVolumeSlider->bounds.y - 15 - Engine::GetInstance().render->camera.y, { 255,255,255,255 });

		generalVolumeSlider->Render();
		musicVolumeSlider->Render();
		sfxVolumeSlider->Render();

	}



	Engine::GetInstance().render->UnlockLayer();


	backButton->Render();
	audioButton->Render();
	videoButton->Render();
}

void SettingsUI::CleanUp()
{
	backButton->CleanUp();
	audioButton->CleanUp();
	videoButton->CleanUp();

	fullscreenToggle->CleanUp();
	generalVolumeSlider->CleanUp();
	sfxVolumeSlider->CleanUp();
	musicVolumeSlider->CleanUp();
	delete backButton;
	delete audioButton;
	delete videoButton;
	delete fullscreenToggle;
	delete generalVolumeSlider;
	delete musicVolumeSlider;
	delete sfxVolumeSlider;
}

void SettingsUI::SetIfOpen(bool p)
{
	isOpen = p;
	audioSettings = false;
	videoSettigns = false;

}

bool SettingsUI::IsOpen()
{
	return isOpen;
}


void SettingsUI::OpenAudioOptions()
{
	audioSettings = true;
	videoSettigns = false;

}

void SettingsUI::OpenVideoOptions()
{
	audioSettings = false;
	videoSettigns = true;

}
