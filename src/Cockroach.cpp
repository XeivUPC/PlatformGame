#include "Cockroach.h"
#include "Engine.h"
#include "Physics.h"
#include "Textures.h"
#include "Box2DCreator.h"

Cockroach::Cockroach(Vector2D pos) : Enemy(pos)
{
	textureOffset = { -16,-8 };
}

Cockroach::~Cockroach()
{
}

void Cockroach::InitAnimations()
{
	textureName = "Assets/Textures/Enemies/Beeto.png";
	Enemy::InitAnimations();

	AnimationData alive = AnimationData("Beeto_Alive");
	alive.AddSprite(Sprite{ texture,{0.0f, 0.0f}, {32, 16}});
	alive.AddSprite(Sprite{ texture,{1.0f, 0.0f}, {32, 16}});
	alive.AddSprite(Sprite{ texture,{2.0f, 0.0f}, {32, 16}});
	alive.AddSprite(Sprite{ texture,{3.0f, 0.0f}, {32, 16}});

	AnimationData dead = AnimationData("Beeto_Dead");
	dead.AddSprite(Sprite{ texture,{0.0f, 1.0f}, {32, 16} });
	dead.AddSprite(Sprite{ texture,{1.0f, 1.0f}, {32, 16} });

	animator->AddAnimation(alive);
	animator->AddAnimation(dead);

	animator->SelectAnimation("Beeto_Alive", true);
	animator->SetSpeed(100);
}

void Cockroach::InitColliders()
{
	const std::shared_ptr<Box2DCreator>& colliderCreator = Engine::GetInstance().box2DCreator;
	b2World* world = Engine::GetInstance().physics->world;
	
	b2Vec2 enemyColliderPosition{ PIXEL_TO_METERS(position.getX()), PIXEL_TO_METERS(position.getY()) };
	enemyCollider = colliderCreator->CreateBox(world, enemyColliderPosition, PIXEL_TO_METERS(32), PIXEL_TO_METERS(16));
	enemyCollider->SetFixedRotation(true);

	/*b2FixtureUserData sidePlayerCheckData;
	sidePlayerCheckData.pointer = (uintptr_t)(&sidePlayerCheckController);
	sidePlayerCheck = colliderCreator->AddBox(enemyCollider, b2Vec2(0.0f, PIXEL_TO_METERS(0.5f)), PIXEL_TO_METERS(32), PIXEL_TO_METERS(10), sidePlayerCheckData);
	sidePlayerCheck->SetSensor(true);
	sidePlayerCheck->SetDensity(0);
	sidePlayerCheck->SetFilterData(playerFilter);

	b2FixtureUserData topPlayerCheckData;
	topPlayerCheckData.pointer = (uintptr_t)(&topPlayerCheckController);
	topPlayerCheck = colliderCreator->AddBox(enemyCollider, b2Vec2(0.0f, PIXEL_TO_METERS(0.5f)), PIXEL_TO_METERS(30), PIXEL_TO_METERS(2), sidePlayerCheckData);
	topPlayerCheck->SetSensor(true);
	topPlayerCheck->SetDensity(0);
	topPlayerCheck->SetFilterData(playerFilter);*/

	Enemy::InitColliders();
}

void Cockroach::Brain()
{
	Enemy::Brain();
	
}