#pragma once
#include "Module.h"
#include "SDL2/SDL.h"
#include "Engine.h"
#include "Vector2D.h"
#include "HealthSystem.h"

class UIGroup
{
private:
	Vector2D offset;
public:
	bool isActive = false;
	void Activate();
	void Deactivate();
	virtual void Render() = 0;
};

class TopUI : public UIGroup
{
public:
	void OrbDrawAndLogic(Health counter, int factor, int fullIndex, int halfIndex, int emptyIndex, Vector2D offset);
	void Render()override;
};

class Pause : public UIGroup
{
public:
	
	void Render()override;
};

class Dialogue : public UIGroup
{
public:
	void Render()override;
};

class UI : public Module
{
private:
	SDL_Texture* UITexture = NULL;
	TopUI topUI;
	Pause pauseMenuUI;
	Dialogue dialogueUI;
	enum Constraint
	{
		OPTION_CONSTRAINT_HORIZONTAL,
		OPTION_CONSTRAINT_VERTICAL,
		OPTION_CONSTRAINT_GRID,
	};
public:
	UI();
	virtual ~UI();
	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	SDL_Texture* GetTexture();
	void DrawOnOption(SDL_Rect* rect, int option, Vector2D scale, Vector2D count, Vector2D offset, Constraint constraint);

};