#pragma once
#include "Item.h"
#include "AnimationSystem.h"



class Healthtem : Item
{
public:
	Healthtem(Vector2D position, int id);
	virtual ~Healthtem();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
protected:
	void Pick();

private:
	Animator* animator = nullptr;
};