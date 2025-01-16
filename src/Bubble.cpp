#include "Bubble.h"
#include "Engine.h"
#include "Physics.h"
#include "Textures.h"
#include "Box2DFactory.h"
#include "Debug.h"
#include "Log.h"
#include <random>
#include "Box2DRender.h"

Bubble::Bubble() : Entity(EntityType::UNKNOWN)
{
	animator = new Animator();
	texture = Engine::GetInstance().textures->GetTexture("BubbleTexture");
	AnimationData idle = AnimationData("Bubble_Idle");
	AnimationData explosion = AnimationData("Bubble_Explosion");
	Sprite bubble1 = Sprite{ texture, {0,0},{37,35}};
	Sprite bubble2 = Sprite{ texture, {1,0},{37,35}};
	Sprite bubble3 = Sprite{ texture, {2,0},{37,35}};
	Sprite bubble4 = Sprite{ texture, {3,0},{37,35}};
	Sprite explosion1 = Sprite{ texture, {0,1},{37,35}};
	Sprite explosion2 = Sprite{ texture, {0,2},{37,35}};
	idle.AddSprite(bubble1);
	idle.AddSprite(bubble2);
	idle.AddSprite(bubble3);
	idle.AddSprite(bubble4);
	explosion.AddSprite(explosion1);
	explosion.AddSprite(explosion2);
	animator->AddAnimation(idle);
	animator->AddAnimation(explosion);
	animator->SelectAnimation("Bubble_Idle", true);
	InitCollisions();
}

Bubble::~Bubble()
{
}

void Bubble::Throw(Vector2D position, Player* p)
{
	player = p;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, 10);
	randomVariation = distr(gen);
	body->SetTransform(b2Vec2(position.getX(), position.getY()), 0);
	inUse = true;
	projectileTimer.Start();
	animator->SelectAnimation("Bubble_Idle", true);
	animator->SetSpeed(250);
}

bool Bubble::Update(float dt)
{
	if (!inUse)
		return true;

	animator->Update(dt);
	if (projectileTimer.ReadMSec() <= projectileLifeMS / 2 + randomVariation*10)
	{
		body->SetLinearVelocity(b2Vec2(-throwVelocity, 0));
	}
	else if (projectileTimer.ReadMSec() <= projectileLifeMS)
	{
		body->SetLinearVelocity(b2Vec2(0, -floatVelocity));
	}
	else
	{
		animator->SelectAnimation("Bubble_Explosion", false);
		if (animator->AnimationEnded("Bubble_Explosion"))
			inUse = false;
	}
	return true;
}

bool Bubble::Render()
{
	if (!inUse)
		return true;
	Engine::GetInstance().render->SelectLayer(Render::RenderLayers::Layer3);
	animator->Animate(METERS_TO_PIXELS(body->GetPosition().x) - 37/2, METERS_TO_PIXELS(body->GetPosition().y) - 35/2, SDL_FLIP_NONE);
	if (playerCheckController.IsBeingTriggered())
	{
		player->Damage(1);
	}
	if (Engine::GetInstance().debug->HasDebug(1))
	{
		Engine::GetInstance().render->LockLayer(Render::RenderLayers::Layer6);
		Box2DRender::GetInstance().RenderBody(body, { 255,0,0,255 });
		Engine::GetInstance().render->UnlockLayer();
	}
	return true;
}

bool Bubble::CleanUp()
{
	LOG("Cleanup bubble");
	delete animator;
	animator = nullptr;
	Engine::GetInstance().physics->world->DestroyBody(body);
	//Engine::GetInstance().textures->UnLoad(texture);
	return true;
}

void Bubble::InitCollisions()
{
	playerFilter.categoryBits = Engine::GetInstance().ENEMY_ATTACK_LAYER;
	playerFilter.maskBits = Engine::GetInstance().PLAYER_LAYER;
	b2FixtureUserData playerCheckData;
	playerCheckData.pointer = (uintptr_t)(&playerCheckController);
	Box2DFactory& colliderCreator = Box2DFactory::GetInstance();
	b2World* world = Engine::GetInstance().physics->world;
	body = colliderCreator.CreateCircle(world, { 0,0 }, 0.5f,playerCheckData);
	body->SetFixedRotation(true);
	body->GetFixtureList()[0].SetFilterData(playerFilter);
	body->GetFixtureList()[0].SetSensor(true);
	body->SetGravityScale(0);

	

	playerCheckController.SetBodyToTrack(&body->GetFixtureList()[0]);
}

bool Bubble::InUse()
{
	return inUse;
}
