#pragma once
#include "Enemy.h"
class Divedrake : public Enemy
{
public:
	Divedrake(Vector2D pos, LevelSection* layer);
	~Divedrake();

private:
	void LoadParameters();
	void InitColliders();
	void Brain();
	void Render(float dt);
};