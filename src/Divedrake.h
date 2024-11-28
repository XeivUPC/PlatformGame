#pragma once
#include "Enemy.h"
class Divedrake : public Enemy
{
public:
	Divedrake(Vector2D pos, MapLayer* layer);
	~Divedrake();

private:
	void InitAnimations();
	void InitColliders();
	void Brain();
	void Render(float dt);
};