#pragma once
#include "Entity.h"
#include "SDL2/SDL.h"
#include <box2d/box2d.h>
#include "Timer.h"
#include "Player.h"
#include "AnimationSystem.h"
#include "CollisionSensor.h"
#include "HealthSystem.h"

class Enemy : public Entity
{
private:
	//Physics
	float baseGravity = 1.0f;

protected:
	//Game Parameters
	int hitDamage = 1;
	float speed = 1.0f;
	int attackCooldownMS = 500;
	int hurtCooldownMS = 500;
	Timer attackCooldown;
	Timer hurtCooldown;
	Health enemyHealth = Health(1);
	Player* player;
	Vector2D enemyDirection;

	//Render
	std::string textureName;
	SDL_Texture* texture = NULL;
	Animator* animator;
	Vector2D textureOffset;
	virtual void InitAnimations();

	//Colliders
	virtual void InitColliders();

		//Enemy Collider
		float enemyMass = 1.0f;
		b2Body* enemyCollider;

		//Ground Collider
		b2Fixture* groundCheck;
		CollisionSensor groundCheckController;

		//Direction Collider
		b2Fixture* directionRightCheck;
		b2Fixture* directionLeftCheck;
		CollisionSensor directionRightCheckController;
		CollisionSensor directionLeftCheckController;

		//Player Collider
		b2Fixture* playerCheck;
		CollisionSensor playerCheckController;

		//Filters
		b2Filter groundFilter;
		b2Filter playerFilter;

	//Functions
	void Attack();
	void Hurt();
	void Die();
	Vector2D SeekForSurfaceTile();
	void Move();
	Vector2D TrackPlayerPosition(bool verticalAxis, bool horizontalAxis);
	virtual void Brain();
	virtual void Render(float dt);

public:
	Enemy(Vector2D pos);
	virtual ~Enemy();
	bool Awake();
	bool Start();
	virtual bool Update(float dt);
	bool CleanUp();
};
	
	
