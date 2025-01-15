#include "IntroScene.h"
#include "Engine.h"
#include "Input.h"
#include "TitleScene.h"
#include "Log.h"
#include "FadeUI.h"

IntroScene::IntroScene(bool isActive) : Module(isActive)
{
	name = "IntroScene";
}

IntroScene::~IntroScene()
{
}

bool IntroScene::Awake()
{
	LOG("Loading IntroScene");
	return true;
}

bool IntroScene::Start()
{
	ui = new IntroUI(this);
	fade = new FadeUI(this);
	fade->StartFadeOut({ 0,0,0,255 }, 500.0f);
	return true;
}

bool IntroScene::PreUpdate()
{

	return true;
}

bool IntroScene::Update(float dt)
{
	ui->Update(dt);
	fade->Update(dt);
	if (timer.ReadMSec() > timeInIntro) {
		if (!timerFinished)
			fade->StartFadeIn({ 0,0,0,255 }, 500.0f);
		timerFinished = true;
		if (fade->IsFading())
			return true;

		Engine::GetInstance().title_scene->Enable();
		Disable();

	}
	return true;
}

bool IntroScene::PostUpdate()
{
	
	return true;
}

bool IntroScene::CleanUp()
{
	LOG("Freeing IntroScene");
	ui->CleanUp();
	fade->CleanUp();
	delete ui;
	delete fade;
	return true;
}

bool IntroScene::LoadParameters(xml_node parameters)
{
	return true;
}
