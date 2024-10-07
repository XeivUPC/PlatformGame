#pragma once

#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include <box2d/box2d.h>

class Box2DCreator //factory Box2DFactory --> PhysicsFactory
{
public:

	b2Body* CreateCircle(b2World* world, b2Vec2 position,float radius);
	b2Body* CreateBox(b2World* world, b2Vec2 position, float width, float height);
	b2Body* CreateCapsule(b2World* world, b2Vec2 position, float width, float height, float radius);

	b2Fixture* AddCircle(b2Body* bodyToAddTo, b2Vec2 offset, float radius);
	b2Fixture* AddBox(b2Body* bodyToAddTo, b2Vec2 offset, float width, float height);
	b2Fixture* AddCapsule(b2Body* bodyToAddTo, b2Vec2 offset, float width, float height, float radius);

	void RenderBody(b2Body* body, b2Color color);
	void RenderFixture(b2Fixture* body, b2Color color);

private:
	b2CircleShape CreateCircleShape(float radius, b2Vec2 offset = b2Vec2_zero);
	b2PolygonShape CreateBoxShape(float width, float height, b2Vec2 offset = b2Vec2_zero);
};

