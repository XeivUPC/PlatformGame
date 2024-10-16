#include "Cockroach.h"
#include "Engine.h"
#include "Physics.h"
#include "Box2DCreator.h"

Cockroach::~Cockroach()
{
}

void Cockroach::InitAnimations()
{
	Enemy::InitAnimations();
	AnimationData alive = AnimationData("Beeto_Alive");
	alive.AddSprite(Sprite{ texture,{0.0f, 0.0f}, {64, 32}});
	alive.AddSprite(Sprite{ texture,{1.0f, 0.0f}, {64, 32}});
	alive.AddSprite(Sprite{ texture,{2.0f, 0.0f}, {64, 32}});
	alive.AddSprite(Sprite{ texture,{3.0f, 0.0f}, {64, 32}});

	AnimationData dead = AnimationData("Beeto_Dead");
	dead.AddSprite(Sprite{ texture,{0.0f, 1.0f}, {64, 32} });
	dead.AddSprite(Sprite{ texture,{1.0f, 1.0f}, {64, 32} });
}

void Cockroach::InitColliders()
{
	const std::shared_ptr<Box2DCreator>& colliderCreator = Engine::GetInstance().box2DCreator;
	b2World* world = Engine::GetInstance().physics->world;

	Enemy::InitColliders();
	
	b2Vec2 enemyColliderPosition{ PIXEL_TO_METERS(position.getX()), PIXEL_TO_METERS(position.getY()) };
	enemyCollider = colliderCreator->CreateBox(world, enemyColliderPosition, PIXEL_TO_METERS(64), PIXEL_TO_METERS(32));
	enemyCollider->SetFixedRotation(true);
}
