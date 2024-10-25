#pragma once
#include "Module.h"
#include "Scene.h"
#include "box2d/box2d.h"
#include <vector>

#define MAX_ENEMIES 8;
enum AXIS
{
	AXIS_HORIZONTAL,
	AXIS_VERTICAL,
	AXIS_BOTH,
};

class Pathfinding : public Module
{
private:
	Player* p;
	std::vector<b2Body> enemyPathfinders;
	std::vector<b2Fixture> enemyComplexSensors;
public:
	Pathfinding();
	~Pathfinding();
	virtual bool Awake();
	virtual bool Start();
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();

private:
	void SetAllColliders();
	Vector2D GetFirstTile(Vector2D direction);
	Vector2D DistanceToTile(Vector2D dist, AXIS axis = AXIS_BOTH);
	Vector2D TileToDistance(Vector2D tile, AXIS axis = AXIS_BOTH);
};