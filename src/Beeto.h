#pragma once
#include "Enemy.h"
#include "PathfindingSystem.h"
class Beeto : public Enemy
{
public:
	Beeto(Vector2D pos, MapData* mapData);
	virtual ~Beeto();
	bool Start() override;

private:
	void InitAnimations();
	void InitColliders();
	void Brain();
	void Render(float dt);

	b2Fixture* climbCheck;
	CollisionSensor climbCheckController;

	Pathfinding* pathfinding;
	float pathTimeRecalculation = 2000.f;
	Timer timerPath;

	MapData* mapData;
};