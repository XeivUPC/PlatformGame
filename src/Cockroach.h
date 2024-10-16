#pragma once
#include "Enemy.h"
class Cockroach : public Enemy
{
private:
	Cockroach();
	virtual ~Cockroach();
	void InitAnimations();
	void InitColliders();
	void Brain();
};