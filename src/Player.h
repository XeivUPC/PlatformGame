#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include <box2d/box2d.h>
#include "Box2DSensorController.h"
#include "HealthSystem.h"
#include "AnimationSystem.h"

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

	//Declare player parameters
	float speed = 2.0f;
	std::string textureName; 
	SDL_Texture* texture = NULL;

private:

	bool isGrounded = true;
	bool isFlipped;
	
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