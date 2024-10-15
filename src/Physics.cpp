#include "Physics.h"
#include "CollidersManager.h"
#include "Engine.h"

Physics::Physics()
{
}

Physics::~Physics()
{
	delete world;
}

bool Physics::Awake()
{
	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	const std::shared_ptr<CollidersManager>& box2DSensor = Engine::GetInstance().box2DSensors;
	world->SetContactListener(box2DSensor.get());


	return true;
}

bool Physics::Start()
{
	return true;
}

bool Physics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 8, 3);
	return true;
}

bool Physics::Update(float dt)
{
	return true;
}

bool Physics::PostUpdate()
{
	return true;
}

bool Physics::CleanUp()
{
	return true;
}
