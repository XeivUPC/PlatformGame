#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include <box2d/box2d.h>

class Box2DRender
{
public:
	static Box2DRender& GetInstance();
	void RenderBody(b2Body* body, b2Color color);
	void RenderFixture(b2Fixture* body, b2Color color);

private:
	Box2DRender();

	// Delete copy constructor and assignment operator to prevent copying
	Box2DRender(const Box2DRender&) = delete;
	Box2DRender& operator=(const Box2DRender&) = delete;

};

