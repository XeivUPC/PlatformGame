#pragma once
#include "Enemy.h"
class Beeto : public Enemy
{
public:
	Beeto(Vector2D pos);
	virtual ~Beeto();

private:
	void InitAnimations();
	void InitColliders();
	void Brain();
	void Render(float dt);

	b2Fixture* climbCheck;
	CollisionSensor climbCheckController;
};