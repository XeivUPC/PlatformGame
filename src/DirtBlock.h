#pragma once
#include "Vector2D.h"
#include "Entity.h"
#include "AnimationSystem.h"
#include "Timer.h"
#include <box2d/box2d.h>
#include "CollisionSensor.h"

class DirtBlock : Entity
{


public:

	enum DirtSize
	{
		Small,
		Big
	};

	DirtBlock(DirtSize type,Vector2D position);

	virtual ~DirtBlock();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void Break();

private:
	Vector2D pos;
	DirtSize type;

	Animator* animator;
	SDL_Texture* texture;

	Vector2D blockTextureOffset;
	Vector2D particleTextureOffset;

	bool isBroken = false;

	float particleRemoveMS = 300;
	Timer  particleRemoveTimer;

	b2Body* body;
	b2Fixture* hitCheck;
	CollisionSensor collisionController;

	int breakSoundId;
};
