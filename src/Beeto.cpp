#include "Beeto.h"
#include "Engine.h"
#include "Physics.h"
#include "Textures.h"
#include "Box2DFactory.h"
#include "Box2DRender.h"

Beeto::Beeto(Vector2D pos) : Enemy(pos)
{
	textureOffset = { -16,-8 };
	enemyHealth.ModifyBaseHealth(1);
	enemyHealth.ResetHealth();
	speed = 2;
}


Beeto::~Beeto()
{
}

void Beeto::InitAnimations()
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

void Beeto::InitColliders()
{
	Enemy::InitColliders();
	Box2DFactory& colliderCreator = Box2DFactory::GetInstance();
	b2World* world = Engine::GetInstance().physics->world;
	
	b2Vec2 enemyColliderPosition{ (position.getX()), (position.getY()) };
	enemyCollider = colliderCreator.CreateBox(world, enemyColliderPosition, PIXEL_TO_METERS(32), PIXEL_TO_METERS(16));
	enemyCollider->SetFixedRotation(true);
	

	b2FixtureUserData playerCheckData;
	playerCheckData.pointer = (uintptr_t)(&playerCheckController);
	playerCheck = colliderCreator.AddBox(enemyCollider, b2Vec2(0.0f, 0.0f), PIXEL_TO_METERS(28), PIXEL_TO_METERS(12), playerCheckData);
	playerCheck->SetSensor(true);
	playerCheck->SetDensity(0);
	playerCheck->SetFilterData(playerFilter);

	b2FixtureUserData playerDamageCheckData;
	playerDamageCheckData.pointer = (uintptr_t)(&playerDamageCheckController);
	playerDamageCheck = colliderCreator.AddBox(enemyCollider, b2Vec2(0.0f, 0.0f), PIXEL_TO_METERS(36), PIXEL_TO_METERS(20), playerDamageCheckData);
	playerDamageCheck->SetSensor(true);
	playerDamageCheck->SetDensity(0);
	playerDamageCheck->SetFilterData(playerDamageFilter);

	b2FixtureUserData directionRightCheckData;
	directionRightCheckData.pointer = (uintptr_t)(&directionRightCheckController);
	directionRightCheck = colliderCreator.AddBox(enemyCollider, b2Vec2(PIXEL_TO_METERS(24.0f), PIXEL_TO_METERS(16.0f)), PIXEL_TO_METERS(2), PIXEL_TO_METERS(2), directionRightCheckData);
	directionRightCheck->SetSensor(true);
	directionRightCheck->SetDensity(0);
	directionRightCheck->SetFilterData(groundFilter);

	b2FixtureUserData directionLeftCheckData;
	directionLeftCheckData.pointer = (uintptr_t)(&directionLeftCheckController);
	directionLeftCheck = colliderCreator.AddBox(enemyCollider, b2Vec2(PIXEL_TO_METERS(-24.0f), PIXEL_TO_METERS(0.0f)), PIXEL_TO_METERS(2), PIXEL_TO_METERS(2), directionLeftCheckData);
	directionLeftCheck->SetSensor(true);
	directionLeftCheck->SetDensity(0);
	directionLeftCheck->SetFilterData(groundFilter);
	
	b2FixtureUserData directionBottomRightCheckData;
	directionBottomRightCheckData.pointer = (uintptr_t)(&directionBottomRightCheckController);
	directionBottomRightCheck = colliderCreator.AddBox(enemyCollider, b2Vec2(PIXEL_TO_METERS(24.0f), PIXEL_TO_METERS(0.0f)), PIXEL_TO_METERS(2), PIXEL_TO_METERS(2), directionBottomRightCheckData);
	directionBottomRightCheck->SetSensor(true);
	directionBottomRightCheck->SetDensity(0);
	directionBottomRightCheck->SetFilterData(groundFilter);

	b2FixtureUserData directionBottomLeftCheckData;
	directionBottomLeftCheckData.pointer = (uintptr_t)(&directionBottomLeftCheckController);
	directionBottomLeftCheck = colliderCreator.AddBox(enemyCollider, b2Vec2(PIXEL_TO_METERS(-24.0f), PIXEL_TO_METERS(16.0f)), PIXEL_TO_METERS(2), PIXEL_TO_METERS(2), directionBottomLeftCheckData);
	directionBottomLeftCheck->SetSensor(true);
	directionBottomLeftCheck->SetDensity(0);
	directionBottomLeftCheck->SetFilterData(groundFilter);

	playerCheckController.SetBodyToTrack(playerCheck);
	playerDamageCheckController.SetBodyToTrack(playerDamageCheck);
	directionBottomRightCheckController.SetBodyToTrack(directionBottomRightCheck);
	directionBottomLeftCheckController.SetBodyToTrack(directionBottomLeftCheck);
	climbCheckController.SetBodyToTrack(climbCheck);

	enemyCollider->ResetMassData();
	b2MassData massData;
	massData.mass = enemyMass;
	massData.center = enemyCollider->GetLocalCenter();
	enemyCollider->SetMassData(&massData);
}

void Beeto::Brain()
{
	enemyDirection = TrackPlayerPosition(false, true);
	Enemy::Brain();
}

void Beeto::Render(float dt)
{
	Enemy::Render(dt);
	animator->SelectAnimation("Beeto_Alive", true);
}