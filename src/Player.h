#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include <box2d/box2d.h>
#include "CollisionSensor.h"
#include "AnimationSystem.h"
#include "Timer.h"
#include "Counter.h"
#include "HealthSystem.h"

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void Damage(int amount, Vector2D direction = {0,0});

	void SetPosition(Vector2D pos);
	Vector2D GetPosition();

	bool isDoingFallAttack = false;
	bool isDoingShovelAttack = false;


public:

	//Player Exposed Stats
	float speed = 120*3;
	float ladderSpeed = 160;
	float jumpForce = 2.7f;
	float fallAttackJumpMultiplier = 1.0f;

	
	std::string textureName; 
	
	SDL_Texture* texture = NULL;

	Health playerHealth = Health(20);

	Counter coins = Counter();
	Counter magic = Counter();

private:

	bool TryJump();

	void DoJump(float force);

	

	bool TryShovelAttack();
	void DoShovelAttack();

	bool TryFallAttack();
	void DoFallAttack();
	b2Vec2 GetMoveInput();

	void Respawn();

	void SetGravityValue(float verticalVelocity);

	void LoadParameters();

	/// Status
	bool isGrounded = true;
	bool isCrouching = true;
	bool isFlipped=false;
	bool isInLadder = false;
	bool isInvulnerable = false;
	bool isDead=false;

	/// MAX VALUES
	float MAX_FALL_SPEED = 100.0f;

	/// Gravity Values
	float defaultGravity = 1.0f;
	float fallGravity = 2.5f;
	float fallAttackGravity = 2.75f;

	/// Attack Values
	int attackDamage=1;

	/// Coyote Time
	Timer coyoteTimer;
	float coyoteReactionMS = 100;
	

	/// Delays
	float attackRecoverMS=400;
	Timer attackRecoverTimer;

	float exitLadderTimeMS = 100;
	Timer exitLadderTimer;

	float jumpRecoverMS= 150;
	Timer jumpRecoverTimer;

	float hurtAnimEffectMS = 20;
	Timer hurtAnimEffectTimer;
	
	float hurtAnimTimeMS = 2500;
	Timer hurtAnimTimeTimer;

	float deathTimeMS = 4000;
	Timer deathTimeTimer;

	//// Texture
	b2Vec2 textureOffset{-35.0f,-35.0f};

	/// Animator
	Animator* animator;

	//// Audio
	int jumpSoundId;
	int hurtSoundId;
	int dieSoundId;


	//// Colliders
	void InitColliders();
		///// Filters
		b2Filter emptyFilter;

		b2Filter groundCheckFilters;
		b2Filter enemyCheckFilters;
		b2Filter playerFilters;
		b2Filter playerLadderFilters;
			
		///// GroundCollider
		b2Fixture* groundCheck;
		CollisionSensor groundCheckController;
		

		//// EnemyCollider
		b2Fixture* shovelFallAttackCheck;
		CollisionSensor shovelFallAttackCheckController;

		b2Fixture* shovelAttackCheckLeft;
		b2Fixture* shovelAttackCheckRight;

		//// LadderCollider
		b2Fixture* ladderCheck;
		CollisionSensor ladderCheckController;

		///// Player Collider
		float playerMass = 0.15f;
		b2Body* playerCollider;
};