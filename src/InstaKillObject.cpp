#include "InstaKillObject.h"
#include "Engine.h"
#include "Physics.h"
#include "Box2DFactory.h"
#include "Box2DRender.h"
#include "CollisionsManager.h"
#include "EntityManager.h"
#include "Debug.h"
#include "GameScene.h"
#include "Player.h"

InstaKillObject::InstaKillObject(Vector2D position, Vector2D size) : Entity(EntityType::UNKNOWN)
{
	this->position = position;
	this->size = size;
}

InstaKillObject::~InstaKillObject()
{
}

bool InstaKillObject::Awake()
{
	return true;
}

bool InstaKillObject::Start()
{
	b2Filter filter;
	filter.categoryBits = Engine::GetInstance().ENEMY_ATTACK_LAYER;
	filter.maskBits = Engine::GetInstance().PLAYER_LAYER;


	Box2DFactory& colliderCreator = Box2DFactory::GetInstance();
	b2World* world = Engine::GetInstance().physics->world;
	b2Vec2 colliderPosition{ (position.getX()), (position.getY()) };
	

	b2FixtureUserData fixtureData;
	fixtureData.pointer = (uintptr_t)(&collisionController);
	body = colliderCreator.CreateBox(world, colliderPosition, (size.getX()), (size.getY()), fixtureData);
	body->GetFixtureList()[0].SetFilterData(filter);
	body->SetType(b2_staticBody);

	body->GetFixtureList()[0].SetSensor(true);


	collisionController.SetBodyToTrack(&body->GetFixtureList()[0]);




	return true;
}

bool InstaKillObject::Update(float dt)
{
	if (collisionController.OnTriggerEnter()) {
		Engine::GetInstance().game_scene->player->Damage(Engine::GetInstance().game_scene->player->playerHealth.GetCurrentHealth());
	}

	
	return true;
}

bool InstaKillObject::Render()
{
	if (Engine::GetInstance().debug->HasDebug(1))
	{
		Engine::GetInstance().render->LockLayer(Render::RenderLayers::Layer7);
		Box2DRender::GetInstance().RenderBody(body, { 255,0,0,255 });
		Engine::GetInstance().render->UnlockLayer();
	}
	return true;
}

bool InstaKillObject::CleanUp()
{
	if (body != nullptr)
		Engine::GetInstance().physics->world->DestroyBody(body);
	return true;
}
