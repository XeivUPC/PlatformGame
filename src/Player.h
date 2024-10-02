#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include <box2d/box2d.h>
#include "Box2DSensorController.h"
#include "Timer.h"

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
	float speed = 2.5f;
	float jumpForce = 85.0f;

	
	std::string textureName; 
	SDL_Texture* texture = NULL;

private:

	bool TryJump();
	void DoJump();

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

	/// MAX VALUES
	const float MAX_FALL_SPEED = 100.0f;

	/// Gravity Values
	float defaultGravity = 1.0f;
	float fallGravity = 2.5f;
	float fallAttackGravity = 3.5f;

	/// Attack Values
	int attackDamage=1;
	

	/// Delays

	float attackRecoverMS=300;
	Timer attackRecoverTimer;

	float jumpRecoverMS= 150;
	Timer jumpRecoverTimer;
	
	//// Texture
	b2Vec2 textureOffset{-33/2.f,-31/2.f};


	//// Colliders
	void InitColliders();
		///// GroundCollider
		b2Body* groundCheck;
		b2Vec2 groundCheckOffset;
		Box2DSensorController groundCheckController;

		///// GroundCollider
		b2Body* playerCollider;
};