#include "Engine.h"
#include "Input.h"
#include "Textures.h"
#include "Scene.h"
#include "Log.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Player.h"
#include "Window.h"
#include "Render.h"
#include "PauseUI.h"
#include "GuiControlButton.h"


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
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	pause = new PauseUI(this);
	return true;
}

// Load Parameters from config file
bool Scene::LoadParameters(xml_node parameters) {

	configParameters = parameters;
	return true;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	if (control == (GuiControl*)pause->pauseButton)
	{
		pause->SetPause(true);
	}
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
	pause->Update(dt);
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
