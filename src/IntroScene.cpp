#include "IntroScene.h"
#include "Engine.h"
#include "Input.h"
#include "TitleScene.h"
#include "Log.h"

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
	return true;
}

bool IntroScene::PreUpdate()
{

	return true;
}

bool IntroScene::Update(float dt)
{
	ui->Update(dt);
	if (timer.ReadMSec() > timeInIntro) {
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
	delete ui;
	return true;
}

bool IntroScene::LoadParameters(xml_node parameters)
{
	return true;
}
