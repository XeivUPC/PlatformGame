#include "Box2DCreator.h"
#include "Engine.h"
#include "Scene.h"
#include "Render.h"
#include "Window.h"

b2Body* Box2DCreator::CreateCircle(b2World* world, b2Vec2 position, float radius)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	b2Body* body = world->CreateBody(&bodyDef);

	b2CircleShape circleShape = CreateCircleShape(radius);

	b2FixtureDef topCircleFixtureDef;
	topCircleFixtureDef.shape = &circleShape;
	topCircleFixtureDef.density = 1.0f;
	topCircleFixtureDef.friction = 0.3f;
	body->CreateFixture(&topCircleFixtureDef);

	return body;
}

b2Body* Box2DCreator::CreateBox(b2World* world, b2Vec2 position, float width, float height)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	b2Body* body = world->CreateBody(&bodyDef);

	b2PolygonShape boxShape = CreateBoxShape(width, height);

	b2FixtureDef topCircleFixtureDef;
	topCircleFixtureDef.shape = &boxShape;
	topCircleFixtureDef.density = 1.0f;
	topCircleFixtureDef.friction = 0.3f;
	body->CreateFixture(&topCircleFixtureDef);

	return body;
}

b2Body* Box2DCreator::CreateCapsule(b2World* world, b2Vec2 position, float width, float height, float radius)
{
	float rectangleHeight = height - 2 * radius; // Adjust height to accommodate the circle ends

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	b2Body* body = world->CreateBody(&bodyDef);

	b2CircleShape circleShape = CreateCircleShape(radius);
	b2PolygonShape boxShape = CreateBoxShape(width, rectangleHeight);

	// Create the rectangle fixture
	b2FixtureDef rectangleFixtureDef;
	rectangleFixtureDef.shape = &boxShape;
	rectangleFixtureDef.density = 1.0f;
	rectangleFixtureDef.friction = 0.3f;

	body->CreateFixture(&rectangleFixtureDef);

	// Top circle
	b2FixtureDef topCircleFixtureDef;
	topCircleFixtureDef.shape = &circleShape;
	topCircleFixtureDef.density = 1.0f;
	topCircleFixtureDef.friction = 0.3f;
	circleShape.m_p.Set(0, rectangleHeight / 2.0f); // Move the circle to the top
	body->CreateFixture(&topCircleFixtureDef);

	// Bottom circle
	b2FixtureDef bottomCircleFixtureDef;
	bottomCircleFixtureDef.shape = &circleShape;
	bottomCircleFixtureDef.density = 1.0f;
	bottomCircleFixtureDef.friction = 0.3f;
	circleShape.m_p.Set(0, -rectangleHeight / 2.0f); // Move the circle to the bottom
	body->CreateFixture(&bottomCircleFixtureDef);

	return body;

}

void Box2DCreator::RenderBody(b2Body* body, b2Color color)
{
	b2World* world = Engine::GetInstance().scene.get()->world;
	Render* render = Engine::GetInstance().render.get();

	for (b2Fixture* fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
		// Get the shape type (polygon or circle)
		b2Shape::Type shapeType = fixture->GetType();

		// Handle polygon shapes (for rectangles)
		if (shapeType == b2Shape::e_polygon) {
			// Cast to a polygon shape
			b2PolygonShape* polygonShape = (b2PolygonShape*)fixture->GetShape();

			// Get the body transform (position and rotation)
			const b2Transform& transform = body->GetTransform();

			// Get vertices of the polygon
			SDL_Point points[b2_maxPolygonVertices];  // Max number of vertices in a Box2D polygon
			int vertexCount = polygonShape->m_count;

			// Convert Box2D world coordinates to screen coordinates using the body's transform
			for (int i = 0; i < vertexCount; ++i) {
				b2Vec2 vertex = b2Mul(transform, polygonShape->m_vertices[i]);
				points[i].x = (int)(METERS_TO_PIXELS(vertex.x));
				points[i].y = (int)(METERS_TO_PIXELS(vertex.y));
			}

			// Render the polygon as a filled shape using DrawLine to connect vertices
			for (int i = 0; i < vertexCount; ++i) {
				int next = (i + 1) % vertexCount;  // Wrap around to the first vertex
				render->DrawLine(points[i].x, points[i].y, points[next].x, points[next].y, color.r, color.g, color.b, color.a, false);
			}

		}
		else if (shapeType == b2Shape::e_circle) {
			// Cast to a circle shape
			b2CircleShape* circleShape = (b2CircleShape*)fixture->GetShape();

			// Get the circle's position in world coordinates
			b2Vec2 center = body->GetWorldPoint(circleShape->m_p);
			float radius = circleShape->m_radius;

			// Convert Box2D world coordinates to screen coordinates
			int screenX = (int)(METERS_TO_PIXELS(center.x));
			int screenY = (int)(METERS_TO_PIXELS(center.y));
			int screenRadius = (int)(METERS_TO_PIXELS(radius));

			int scale = Engine::GetInstance().window.get()->GetScale();
			// Draw the circle using the custom DrawCircle method
			render->DrawCircle(screenX, screenY, screenRadius* scale, color.r, color.g, color.b, color.a, false);
		}
	}
}


b2CircleShape Box2DCreator::CreateCircleShape(float radius)
{
	b2CircleShape circle;
	circle.m_radius = radius;
	return circle;
}

b2PolygonShape Box2DCreator::CreateBoxShape(float width, float height)
{
	b2PolygonShape rectangle;
	rectangle.SetAsBox(width / 2.0f, height / 2.0f);
	return rectangle;
}
