#pragma once
#include "Item.h"
#include "AnimationSystem.h"



class GoldItem : Item
{
public:
	GoldItem(Vector2D position, int id);
	virtual ~GoldItem();
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();
protected:
	void Pick();

private:
	Animator* animator = nullptr;
};