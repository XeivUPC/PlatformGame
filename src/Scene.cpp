#include "Engine.h"
#include "Input.h"
#include "Textures.h"
#include "Scene.h"
#include "Log.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Player.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Window.h"
#include "Render.h"


Scene::Scene() : Module()
{
	name = "scene";
}

// Destructor
Scene::~Scene()
{
	
}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	//Get the player texture name from the config file and assigns the value	
	//Instantiate the player using the entity manager
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
	pauseButton = Engine::GetInstance().ui->CreateGuiControl(GuiControlType::BUTTON, 33, "", {Engine::GetInstance().window->width-20,10,10,10}, this);
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	return true;
}

// Load Parameters from config file
bool Scene::LoadParameters(xml_node parameters) {

	configParameters = parameters;
	return true;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	if (control == pauseButton)
	{

	}
		//pause
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	//if(Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN){
	//	if (player->active == true) player->Disable();
	//	else player->Enable();
	//}
	Engine::GetInstance().render->SelectLayer(Render::Layer6);
	switch (pauseButton->CurrentState())
	{
	case GuiControlState::DISABLED:
		Engine::GetInstance().render->DrawRectangle({ Engine::GetInstance().window->width-20, 10, 10, 10}, 127, 127, 127, 255, true, false);
		break;
	case GuiControlState::NORMAL:
		Engine::GetInstance().render->DrawRectangle({ Engine::GetInstance().window->width - 20, 10, 10, 10 }, 255, 255, 255, 255, true, false);
		break;
	case GuiControlState::FOCUSED:
		Engine::GetInstance().render->DrawRectangle({ Engine::GetInstance().window->width - 20, 10, 10, 10 }, 255, 0, 0, 255, true, false);
		break;
	case GuiControlState::PRESSED:
		Engine::GetInstance().render->DrawRectangle({ Engine::GetInstance().window->width - 20, 10, 10, 10 }, 0, 255, 0, 255, true, false);
		break;
	case GuiControlState::SELECTED:
		Engine::GetInstance().render->DrawRectangle({ Engine::GetInstance().window->width - 20, 10, 10, 10 }, 0, 0, 255, 255, true, false);
		break;
	}
	if (isPaused)
	{
		Engine::GetInstance().render->DrawRectangle({ 10,10,Engine::GetInstance().window->width, Engine::GetInstance().window->height },255,255,255,255,true,false);
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	// Detects if the player wants to exit the game with ESC key
	if(Engine::GetInstance().input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
