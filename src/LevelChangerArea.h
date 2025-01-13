#pragma once
#include "Vector2D.h"
#include "Entity.h"
#include <box2d/box2d.h>
#include "CollisionSensor.h"
class LevelChangerArea : public Entity
{
public:
	LevelChangerArea(Vector2D position, Vector2D size, int levelToMoveAt);
	virtual ~LevelChangerArea();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
protected:
	void ChangeArea();

private:
	int levelTarget = -1;
	b2Body* body = nullptr;
	CollisionSensor collisionController;
	Vector2D size;
};