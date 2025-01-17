#pragma once

#include "Module.h"
#include "Entity.h"
#include <list>

class EntityManager : public Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake();

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type, bool doEarlyCalls=false);

	void DestroyEntity(Entity* entity);
	void DestroyEntityAtUpdateEnd(Entity* entity);

	void AddEntity(Entity* entity, bool doEarlyCalls=false);

public:
	bool paused = false;
	std::list<Entity*> entities;
private:
	std::list<Entity*> entitiesToDestroyAtUpdateEnd;

};
