#include "Divedrake.h"
#include "Engine.h"
#include "Physics.h"
#include "Textures.h"
#include "Box2DFactory.h"
#include "Box2DRender.h"

Divedrake::Divedrake(Vector2D pos, MapLayer* layer) : Enemy(pos, layer)
{
	textureOffset = { -16,-7 };
	enemyHealth.ModifyBaseHealth(1);
	enemyHealth.ResetHealth();
	speed = 80.0f * 3;

	blockedTiles = { 802, 803, 804 };
}

Divedrake::~Divedrake()
{
}

void Divedrake::InitAnimations()
{
	textureName = "Assets/Textures/Enemies/Beeto.png";
	Enemy::InitAnimations();

	AnimationData alive = AnimationData("Beeto_Alive");
	alive.AddSprite(Sprite{ texture,{0.0f, 0.0f}, {32, 16} });
	alive.AddSprite(Sprite{ texture,{1.0f, 0.0f}, {32, 16} });
	alive.AddSprite(Sprite{ texture,{2.0f, 0.0f}, {32, 16} });
	alive.AddSprite(Sprite{ texture,{3.0f, 0.0f}, {32, 16} });

	AnimationData dead = AnimationData("Beeto_Dead");
	dead.AddSprite(Sprite{ texture,{0.0f, 1.0f}, {32, 16} });
	dead.AddSprite(Sprite{ texture,{1.0f, 1.0f}, {32, 16} });

	animator->AddAnimation(alive);
	animator->AddAnimation(dead);

	animator->SelectAnimation("Beeto_Alive", true);
	animator->SetSpeed(100);
}

void Divedrake::InitColliders()
{
	Enemy::InitColliders();
	Box2DFactory& colliderCreator = Box2DFactory::GetInstance();
	b2World* world = Engine::GetInstance().physics->world;

	b2Vec2 enemyColliderPosition{ (position.getX()), (position.getY()) };
	enemyCollider = colliderCreator.CreateBox(world, enemyColliderPosition, PIXEL_TO_METERS(26), PIXEL_TO_METERS(16));
	enemyCollider->SetFixedRotation(true);

	b2Filter enemyFilter;
	enemyFilter.maskBits &= ~Engine::GetInstance().PLAYER_LAYER;
	enemyFilter.maskBits &= ~Engine::GetInstance().ENEMY_ATTACK_LAYER;
	enemyCollider->GetFixtureList()[0].SetFilterData(enemyFilter);


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

	playerCheckController.SetBodyToTrack(playerCheck);

	playerDamageCheckController.SetBodyToTrack(playerDamageCheck);
	directionBottomRightCheckController.SetBodyToTrack(directionBottomRightCheck);
	directionBottomLeftCheckController.SetBodyToTrack(directionBottomLeftCheck);

	enemyCollider->ResetMassData();
	b2MassData massData;
	massData.mass = enemyMass;
	massData.center = enemyCollider->GetLocalCenter();
	enemyCollider->SetMassData(&massData);
}

void Divedrake::Brain()
{
	if (pathUpdateTime < pathUpdateTimer.ReadMSec())
	{
		pathUpdateTimer.Start();
		Engine::GetInstance().pathfinding->FindPath(mapData->tiles, mapData->width, mapData->height, blockedTiles, { position.getX(), position.getY() - 1 }, { player->position.getX(), player->position.getY() - 1 });
		while (!Engine::GetInstance().pathfinding->HasFinished())
		{
			Engine::GetInstance().pathfinding->PropagateAStar(SQUARED);
		}
		if (Engine::GetInstance().pathfinding->HasFound())
		{

		}
		pathData = Engine::GetInstance().pathfinding->GetData();
	}
	FindCurrentTileInPath();
	SetPathDirection();
	Enemy::Brain();
	Enemy::Move();
}

void Divedrake::Render(float dt)
{
	Engine::GetInstance().pathfinding->DrawPath(&pathData);
	Enemy::Render(dt);
	animator->SelectAnimation("Beeto_Alive", true);
}