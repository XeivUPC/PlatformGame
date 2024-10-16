#pragma once
#include "Vector2D.h"
#include "Entity.h"
#include "AnimationSystem.h"
#include "Timer.h"
#include <box2d/box2d.h>
#include "ColliderHandler.h"

class MovingPlatform : Entity
{
public:
	MovingPlatform(int sectionPlaced, Vector2D leftPoint, Vector2D rightPoint, int platformType, bool verticalMovement);

	virtual ~MovingPlatform();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	int GetSection();

private:
	Vector2D leftPoint;
	Vector2D rightPoint;

	bool isVertical = false;
	bool movingBackwards = false;

	int sectionPlaced;
	int platformType=0;

	Animator* animator;
	SDL_Texture* texture;

	Vector2D textureOffset{ -24,-8 };

	float speed = 130;

	b2Body* body;
	ColliderHandler collisionController;
};