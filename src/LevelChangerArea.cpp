#include "LevelChangerArea.h"
#include "Box2DFactory.h"
#include "Engine.h"
#include "LevelManager.h"
#include "Physics.h"
#include "Debug.h"
#include "Box2DRender.h"

LevelChangerArea::LevelChangerArea(Vector2D position, Vector2D size, int levelToMoveAt) : Entity(EntityType::UNKNOWN)
{
	this->position = position;
	this->size = size;
	this->levelTarget = levelToMoveAt;
}

LevelChangerArea::~LevelChangerArea()
{
}

bool LevelChangerArea::Awake()
{
	return true;
}

bool LevelChangerArea::Start()
{
	Box2DFactory& colliderCreator = Box2DFactory::GetInstance();
	b2World* world = Engine::GetInstance().physics->world;

	b2FixtureUserData fixtureData;
	fixtureData.pointer = (uintptr_t)(&collisionController);
	body = colliderCreator.CreateBox(world, { position.getX(), position.getY() }, size.getX(), size.getY(), fixtureData);


	b2Filter filter;
	filter.categoryBits = Engine::GetInstance().INTERACTABLE_LAYER;
	filter.maskBits = Engine::GetInstance().PLAYER_LAYER;

	body->GetFixtureList()[0].SetFilterData(filter);
	body->GetFixtureList()[0].SetSensor(true);
	body->SetType(b2_staticBody);
	collisionController.SetBodyToTrack(&body->GetFixtureList()[0]);

	return true;
}

bool LevelChangerArea::Update(float dt)
{
	if (collisionController.IsBeingTriggered()) {
		ChangeArea();
	}

	if (Engine::GetInstance().debug->HasDebug(1))
	{
		Engine::GetInstance().render->LockLayer(Render::RenderLayers::Layer7);
		Box2DRender::GetInstance().RenderBody(body, { 255,0,0,255 });
		Engine::GetInstance().render->UnlockLayer();
	}

	return true;
}

bool LevelChangerArea::CleanUp()
{
	if (body != nullptr)
		Engine::GetInstance().physics->world->DestroyBody(body);
	return true;
}

void LevelChangerArea::ChangeArea()
{
	Engine::GetInstance().levelManager->LoadLevel(levelTarget);
}
