#pragma once
#include "Vector2D.h"
#include "Entity.h"
#include <box2d/box2d.h>
#include "CollisionSensor.h"
#include "Engine.h"
#include "Physics.h"
#include "EntityManager.h"

class Item : public Entity
{

public:
	Item(Vector2D position, int id) : Entity(EntityType::UNKNOWN) {
		this->position = position;
		this->id = id;
	}

	virtual ~Item() {

	}
	bool Awake() {
		return true;
	}
	bool Start() {
		b2Filter filter;
		filter.categoryBits = Engine::GetInstance().INTERACTABLE_LAYER;
		filter.maskBits = Engine::GetInstance().PLAYER_LAYER;

		body->GetFixtureList()[0].SetFilterData(filter);
		body->GetFixtureList()[0].SetSensor(true);
		body->SetType(b2_staticBody);

		collisionController.SetBodyToTrack(&body->GetFixtureList()[0]);
		return true;
	}
	bool Update(float dt) {
		if (collisionController.IsBeingTriggered()) {
			Pick();
		}
		return true;
	}
	bool CleanUp() { 
		if (body != nullptr)
			Engine::GetInstance().physics->world->DestroyBody(body);
		return true;
	}

protected:
	virtual void Pick() {
		active = false;
	}
	b2Body* body = nullptr;
	SDL_Texture* texture = nullptr;
	Vector2D textureOffset{ 0,0 };
	CollisionSensor collisionController;

};