#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include <box2d/box2d.h>
#include "ColliderHandler.h"
#include "AnimationSystem.h"
#include "Timer.h"
#include "HealthSystem.h"
#include "CounterSystem.h"

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();


public:

	//Player Exposed Stats
	float speed = 8;
	float ladderSpeed = 2;
	float jumpForce = 1800;
	float fallAttackJumpMultiplier = 1.1f;

	
	std::string textureName; 
	
	SDL_Texture* texture = NULL;

	Health playerHealth = Health(3);
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

	void SetGravityValue(float verticalVelocity);

	/// Status
	bool isGrounded = true;
	bool isDoingFallAttack = false;
	bool isDoingShovelAttack = false;
	bool isFlipped=false;
	bool isInLadder = false;

	/// MAX VALUES
	const float MAX_FALL_SPEED = 60.0f;

	/// Gravity Values
	float defaultGravity = 1.0f;
	float fallGravity = 2.25f;
	float fallAttackGravity = 2.5f;

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
	
	//// Texture
	b2Vec2 textureOffset{-35,-35};

	/// Animator
	Animator animator;
	void InitAnimations();


	//// Audio
	int jumpSoundId;


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
		ColliderHandler groundCheckController;
		

		//// EnemyCollider
		b2Fixture* enemyCheck;
		ColliderHandler enemyCheckController;

		//// LadderCollider
		b2Fixture* ladderCheck;
		ColliderHandler ladderCheckController;

		///// Player Collider
		b2Body* playerCollider;
};