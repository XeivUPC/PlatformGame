#pragma once
#include <box2d/box2d.h>
#include "CollisionSensor.h"
#include "Player.h"
#include "Entity.h"
class Timer;
class Bubble : public Entity
{
private:
	Player* player = nullptr;
	b2Filter playerFilter;
	Timer projectileTimer;
	float projectileLifeMS = 1500;
	b2Fixture* playerCheck = nullptr;
	CollisionSensor playerCheckController;
	b2Body* body = nullptr;
	Vector2D direction = Vector2D(0,0);
	float throwVelocity = 10;
	float floatVelocity = 5;
	int randomVariation = 0;
	bool inUse = false;
	Animator* animator = nullptr;
	SDL_Texture* texture = nullptr;
public:
	Bubble();
	~Bubble();
	void Throw(Vector2D position, Player* p);
	bool Update(float dt);
	bool Render();
	bool CleanUp();
	void InitCollisions();
	bool InUse();
};

