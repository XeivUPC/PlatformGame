#pragma once
#include "Enemy.h"
class Cockroach : public Enemy
{
public:
	Cockroach(Vector2D pos);
	virtual ~Cockroach();

private:
	void InitAnimations();
	void InitColliders();
	void Brain();
};