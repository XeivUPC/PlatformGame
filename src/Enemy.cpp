#include "Enemy.h"
#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Box2DCreator.h"
#include "CollidersManager.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "math.h"

void Enemy::InitColliders()
{
	const std::shared_ptr<Box2DCreator>& box2DSensor = Engine::GetInstance().box2DCreator
		;
	b2World* world = Engine::GetInstance().scene->world;

	b2Vec2 enemyColliderposition{ position.getX(), position.getY() };

	b2Filter mapCollidersFilter;
	mapCollidersFilter.categoryBits = Engine::GetInstance().ENEMY_LAYER;
	mapCollidersFilter.maskBits = Engine::GetInstance().GROUND_LAYER;

	b2Filter playerCollidersFilter;
	playerCollidersFilter.categoryBits = Engine::GetInstance().ENEMY_LAYER;
	playerCollidersFilter.maskBits = Engine::GetInstance().PLAYER_LAYER;

	positionCheck->SetSensor(true);
	positionCheck->SetFilterData(mapCollidersFilter);
	positionCheck->SetFriction(0);

	playerCheck->SetSensor(true);
	playerCheck->SetFilterData(mapCollidersFilter);
	playerCheck->SetFriction(0);
	playerCheckController.AcceptOnlyTriggers(false);


	enemyCollider->SetFixedRotation(true);

	Engine::GetInstance().box2DSensors->AddSensor(&positionCheckController);
	Engine::GetInstance().box2DSensors->AddSensor(&playerCheckController);
};

void Enemy::Brain()
{
}

b2Vec2 Enemy::CalculateNearestDirection(bool verticalAxis, bool horizontalAxis)
{
	b2Vec2 nearestDirection;
	float nearestDistance = -1;

	if (verticalAxis)
	{
		if (Engine::GetInstance().scene->player->position.getX() > position.getX())
		{
			nearestDirection = b2Vec2{1, 0};
			nearestDistance = Engine::GetInstance().scene->player->position.getX() - position.getX();
		}
		else
		{
			nearestDirection = b2Vec2{ -1, 0 };
			nearestDistance = position.getX() - Engine::GetInstance().scene->player->position.getX();
		}
	}
	else if (horizontalAxis)
	{
		if (Engine::GetInstance().scene->player->position.getY() > position.getY())
		{
			nearestDirection = b2Vec2{ 0, 1 };
			nearestDistance = Engine::GetInstance().scene->player->position.getY() - position.getY();
		}
		else
		{
			nearestDirection = b2Vec2{ 0, -1};
			nearestDistance = position.getY() - Engine::GetInstance().scene->player->position.getY();
		}
	}
	return nearestDirection;
}

Enemy::Enemy() : Entity(EntityType::UNKNOWN)
{}

Enemy::~Enemy()
{
}

bool Enemy::Awake()
{
	position = Vector2D(0, 0);
	InitColliders();
	positionCheckController.SetSensor(positionCheck);
	playerCheckController.SetSensor(playerCheck);
	attackCooldown = Timer();
	return true;
}

bool Enemy::Start()
{
	texture = Engine::GetInstance().textures->Load(textureName.c_str());

	InitAnimations();

	return true;
}

bool Enemy::Update(float dt)
{
	Brain();
	return true;
}

bool Enemy::CleanUp()
{
	LOG("Cleanup enemy");
	Engine::GetInstance().scene->world->DestroyBody(enemyCollider);
	Engine::GetInstance().textures->UnLoad(texture);
	return true;
}
