#pragma once
#include "Enemy.h"

class Beeto : public Enemy
{
public:
	Beeto(Vector2D pos, LevelSection* layer);
	virtual ~Beeto();

private:
	void LoadParameters();
	void InitColliders();
	void Brain();
	void Render(float dt);
};