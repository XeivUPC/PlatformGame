#pragma once
#include "Vector2D.h"
#include "Entity.h"
#include "AnimationSystem.h"
#include "Timer.h"
#include <box2d/box2d.h>
#include "CollisionSensor.h"

class CheckPoint : Entity
{
public:
	CheckPoint(int sectionPlaced, Vector2D respawnPoint, int id);

	virtual ~CheckPoint();

	bool Awake();

	bool Start();

	bool Update(float dt);
	bool Render();

	bool CleanUp();

	void Unlock();

	bool IsUnlocked();

	int GetSection();
	Vector2D GetRespawnPos();

	void SetExtraData(int value) override;
	int GetExtraData() override;

private:
	Vector2D respawnPos;
	int sectionPlaced;

	Animator* animator;
	SDL_Texture* texture;
	Vector2D textureOffset{ -32,-64 };

	bool isUnlocked = false;

	float turnOnMS = 400;
	Timer  turnOnTimer;

	b2Body* body;
	CollisionSensor collisionController;

	int unlockSoundId;
};

