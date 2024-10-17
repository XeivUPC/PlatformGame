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
	//Physics
	float baseGravity = 1.0f;

protected:
	//Game Parameters
	int hitDamage = 1;
	float speed = 1.0f;
	int attackCooldownMS = 5;
	Timer attackCooldown;
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
		ColliderHandler groundCheckController;

		//Direction Collider
		b2Fixture* directionRightCheck;
		b2Fixture* directionLeftCheck;
		ColliderHandler directionRightCheckController;
		ColliderHandler directionCheckController;

		//Player Side Collider
		b2Fixture* sidePlayerCheck;
		ColliderHandler sidePlayerCheckController;

		//Player Top Collider
		b2Fixture* topPlayerCheck;
		ColliderHandler topPlayerCheckController;

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

public:
	Enemy(Vector2D pos);
	virtual ~Enemy();

	bool Awake();
	bool Start();
	virtual bool Update(float dt);
	bool CleanUp();
};
	
	
