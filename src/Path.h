#include "Vector2D.h"
#include "Enemy.h"
#include <vector>
#pragma once

enum Condition
{
	NONE,
	USING_TERRAIN,
	IMPOSSIBLE,
};

class Path
{
	Vector2D destination;
	Enemy* attachedEnemy;
	void Detach();
public:
	Path(Enemy* enemy, Condition axis_horizontal_condition, Condition axis_vertical_condition, std::vector<Vector2D>tilePath = {});
	void DynamicPathUpdate();

};