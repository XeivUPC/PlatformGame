#pragma once
#include "Entity.h"
#include "SDL2/SDL.h"
#include <box2d/box2d.h>
#include "Timer.h"
#include "Player.h"
#include "AnimationSystem.h"
#include "CollisionSensor.h"
#include "HealthSystem.h"
#include "PathfindingSystem.h"

class Enemy : public Entity
{
private:
	//Physics
	float baseGravity = 1.0f;
	int currentPathTileIndex;

	int dieSoundId;

protected:
	//Game Parameters
	int hitDamage = 1;
	float speed = 100.0f * 3;
	int attackCooldownMS = 500;
	int hurtCooldownMS = 500;
	Vector2D basePosition;
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

	//Path
	std::vector<int> blockedTiles;
	Timer pathUpdateTimer;
	float pathUpdateTime;
	int pathDetectDistance;
	PathData pathData;
	LevelSection* levelSection = nullptr;

	virtual void LoadParameters();

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
		b2Fixture* directionBottomRightCheck;
		b2Fixture* directionBottomLeftCheck;
		CollisionSensor directionRightCheckController;
		CollisionSensor directionLeftCheckController;
		CollisionSensor directionBottomRightCheckController;
		CollisionSensor directionBottomLeftCheckController;

		//Player Collider
		b2Fixture* playerCheck;
		CollisionSensor playerCheckController;
		b2Fixture* playerDamageCheck;
		CollisionSensor playerDamageCheckController;

		//Filters
		b2Filter groundFilter;
		b2Filter playerFilter;
		b2Filter playerDamageFilter;

	//Functions
	void Attack();
	void Hurt();
	void Die();
	void Move();

	void SetPosition(Vector2D pos);
	Vector2D GetPosition();
	virtual void Brain();
	bool Render();
	void SetPathDirection();
	void FindCurrentTileInPath();

public:
	Enemy(Vector2D pos, LevelSection* levelSection, int id);
	virtual ~Enemy();
	bool Awake();
	bool Start();
	virtual bool Update(float dt);
	bool CleanUp();
};