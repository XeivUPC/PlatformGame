#pragma once
#include "Enemy.h"

class Beeto : public Enemy
{
public:
	Beeto(Vector2D pos, LevelSection* layer, int id);
	virtual ~Beeto();

private:
	void LoadParameters();
	void InitColliders();
	void Brain();
	bool Render();
};