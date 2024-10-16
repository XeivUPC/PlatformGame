#pragma once
#include "Entity.h"
#include "SDL2/SDL.h"
#include <box2d/box2d.h>
#include "Timer.h"
#include "Player.h"
#include "AnimationSystem.h"
#include "ColliderHandler.h"
#include "HealthSystem.h"

class Enemy : public Entity
{
private:
	bool canMoveSameDirection = true;
	bool isFlipped = false;

	float baseGravity = 1.0f;
	int baseDamage = 1;
	float attackRecoverMS = 50;
	b2Vec2 textureOffset{ 0,0 };
	Timer attackCooldown;

	Player* player;

protected:
	Animator animator;
	virtual void InitAnimations() = 0;

	void InitColliders();
	b2Fixture* positionCheck;
	ColliderHandler positionCheckController;

	b2Fixture* playerCheck;
	ColliderHandler playerCheckController;

	b2Body* enemyCollider;

	virtual void Brain();
	b2Vec2 CalculateNearestDirection(bool verticalAxis, bool horizontalAxis);
	void ChangeDirection(bool canGoLeft, bool canGoUp, bool canGoRight, bool canGoDown);

public:
	Enemy();
	virtual ~Enemy();

	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	float speed = 120.0f;
	std::string textureName;
	SDL_Texture* texture = NULL;
	Health EnemyHealth = Health(1);
};