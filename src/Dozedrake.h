#pragma once
#include "Enemy.h"
#include "Bubble.h"
class Dozedrake : public Enemy
{
public:
	Dozedrake(Vector2D pos, LevelSection* layer, int id);
	virtual ~Dozedrake();

private:
	std::vector<Bubble*> bubbles;
	float nearDistance;
	float farDistance;
	b2Fixture* playerHeadCheck;
	CollisionSensor playerHeadCheckController;
	b2Fixture* playerHeadDamageCheck;
	CollisionSensor playerHeadDamageCheckController;
	b2Fixture* backwardLimitCheck;
	CollisionSensor backwardLimitCheckController;
	Vector2D mouthPosition = Vector2D(0, 0);
	bool isAsleep = true;
	bool isAttacking = false;
	bool isBeingHurt = false;
	float shootSpacingMS = 500;
	Timer shootSpacing;
	void LoadParameters();
	void InitColliders();
	void Brain();
	void Render(float dt);
	bool CleanUp();
	void Shoot(Bubble* b);
	Bubble* GetBubbleToShoot();
};