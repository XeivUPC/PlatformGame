#pragma once
#include "Enemy.h"
#include "PathfindingSystem.h"
class Beeto : public Enemy
{
public:
	Beeto(Vector2D pos, MapLayer* layer);
	virtual ~Beeto();

private:
	std::vector<int> blockedTiles = { 801, 803};
	PathData pathData;
	MapLayer* mapData = nullptr;
	Timer pathUpdateTimer;
	float pathUpdateTime = 2000;
	void InitAnimations();
	void InitColliders();
	void Brain();
	void Render(float dt);

	b2Fixture* climbCheck;
	CollisionSensor climbCheckController;
};