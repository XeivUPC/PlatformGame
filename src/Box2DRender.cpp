#include "Box2DRender.h"
#include "Render.h"
#include "Engine.h"
#include "Physics.h"

Box2DRender::Box2DRender()
{
}


Box2DRender& Box2DRender::GetInstance()
{
	static Box2DRender instance; // Guaranteed to be destroyed and instantiated on first use
	return instance;
}

void Box2DRender::RenderBody(b2Body* body, b2Color color)
{
	b2World* world = Engine::GetInstance().physics->world;
	const std::shared_ptr<Render>& render = Engine::GetInstance().render;

	for (b2Fixture* fixture = body->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext()) {
		// Get the shape type (polygon or circle)
		RenderFixture(fixture, color);
	}
}

void Box2DRender::RenderFixture(b2Fixture* fixture, b2Color color)
{
	b2Body* body = fixture->GetBody();
	b2Shape::Type shapeType = fixture->GetType();
	const std::shared_ptr<Render>& render = Engine::GetInstance().render;

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
			render->DrawLine(points[i].x, points[i].y, points[next].x, points[next].y, color.r, color.g, color.b, color.a, true);
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

		// Draw the circle using the custom DrawCircle method
		render->DrawCircle(screenX, screenY, screenRadius, color.r, color.g, color.b, color.a, true);
	}
}

