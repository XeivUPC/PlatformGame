#include "EntityManager.h"
#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Log.h"


EntityManager::EntityManager() : Module()
{
	name = "entitymanager";

}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake()
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;

	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->CleanUp();
		delete entity;
	}
	entities.clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type, bool doEarlyCalls)
{
	Entity* entity = nullptr; 

	//L03: TODO 3a: Instantiate entity according to the type and add the new entity to the list of Entities
	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	default:
		break;
	}

	entities.push_back(entity);

	if (doEarlyCalls) {
		entity->Awake();
		entity->Start();
	}

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		if (*it == entity) {
			Entity* entity = *it;
			entities.erase(it); // Remove the entity from the list
			entity->CleanUp();
			delete entity; // Free the allocated memory
			entity = nullptr;
			break; // Exit the loop after removing the entity
		}
	}
}

void EntityManager::DestroyEntityAtUpdateEnd(Entity* entity)
{
	entitiesToDestroyAtUpdateEnd.emplace_back(entity);
}

void EntityManager::AddEntity(Entity* entity, bool doEarlyCalls)
{
	if (entity != nullptr) {
		entities.push_back(entity);
		if (doEarlyCalls) {
			entity->Awake();
			entity->Start();
		}
	}
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	for(const auto entity : entities)
	{
		if (entity == nullptr) continue;
		if (entity->active == false) continue;
		ret = entity->Update(dt);
	}

	for (const auto entityToDelete : entitiesToDestroyAtUpdateEnd)
	{
		DestroyEntity(entityToDelete);
	}
	entitiesToDestroyAtUpdateEnd.clear();
	return ret;
}