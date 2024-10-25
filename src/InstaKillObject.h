#pragma once
#include "Vector2D.h"
#include "Entity.h"
#include "AnimationSystem.h"
#include "Timer.h"
#include <box2d/box2d.h>
#include "CollisionSensor.h"

class InstaKillObject : Entity
{


public:

	InstaKillObject(Vector2D position, Vector2D size);

	virtual ~InstaKillObject();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

private:
	Vector2D position;
	Vector2D size;


	b2Body* body;
	CollisionSensor collisionController;

};
