#pragma once

#include "Input.h"
#include "Render.h"

enum class EntityType
{
	PLAYER,
	UNKNOWN
};

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {  }
	Entity(EntityType type, int id) : id(id), type(type), active(true) {  }

	virtual ~Entity() {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Render()
	{
		return true;
	}


	virtual bool CleanUp()
	{
		return true;
	}

	void Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void SetPosition(Vector2D pos) {
		position = pos;
	}

	virtual Vector2D GetPosition() {
		return position;
	}
	virtual int GetExtraData() { return -1; }
	virtual void SetExtraData(int value){}

public:

	std::string name;
	int id=-1;
	EntityType type;
	bool active = true;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	Vector2D position;       
	bool renderable = true;

};