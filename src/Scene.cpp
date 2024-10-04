#include "Engine.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "LevelManager.h"
#include "Scene.h"
#include "Log.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Box2DCreator.h"
#include "CollidersManager.h"
#include "Player.h"


Scene::Scene() : Module()
{
	name = "scene";
}

// Destructor
Scene::~Scene()
{
	delete world;
}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	//Get the player texture name from the config file and assigns the value

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	world->SetContactListener(Engine::GetInstance().box2DSensors.get());

	//Instantiate the player using the entity manager
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
	player->textureName = configParameters.child("player").attribute("texturePath").as_string();

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

// Called each loop iteration
bool Scene::PreUpdate()
{
	world->Step(1.0f / 60.0f, 8, 3);
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if(Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_E) == KEY_DOWN){
		if (player->active == true) player->Disable();
		else player->Enable();
	}


	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	// Detects if the player wants to exit the game with ESC key
	if(Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
