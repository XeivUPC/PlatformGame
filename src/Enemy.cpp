#include "Enemy.h"
#include "Engine.h"
#include "Physics.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Textures.h"
#include "Window.h"

void Enemy::InitColliders()
{
	const std::shared_ptr<Box2DCreator>& colliderCreator = Engine::GetInstance().box2DCreator;
	b2World* world = Engine::GetInstance().physics->world;

	enemyCollider->ResetMassData();

	b2MassData massData;
	massData.mass = enemyMass;
	massData.center = enemyCollider->GetLocalCenter();
	enemyCollider->SetMassData(&massData);
}

void Enemy::Attack()
{
	player->playerHealth.Hurt(hitDamage);
}

void Enemy::Hurt()
{
	enemyHealth.Hurt(1);
	if (!enemyHealth.IsAlive())
		Die();
}

void Enemy::Die()
{
	Engine::GetInstance().entityManager->DestroyEntityAtUpdateEnd(this);
}

Vector2D Enemy::SeekForSurfaceTile()
{
	return{ 0,0 };
}

void Enemy::Move()
{
}

Vector2D Enemy::TrackPlayerPosition(bool verticalAxis, bool horizontalAxis)
{
	Vector2D playerPosition = position;
	if (verticalAxis)
	{
		playerPosition.setY(player->position.getY());
	}
	if (horizontalAxis)
	{
		playerPosition.setX(player->position.getX());
	}
	return playerPosition;
}

void Enemy::Brain()
{
	if (sidePlayerCheckController.OnTriggerEnter())
	{
		if (!player->isDoingShovelAttack && attackCooldown.ReadSec() == 0)
			Attack();
		else if (player->isDoingShovelAttack)
			Hurt();
	}
	else if (topPlayerCheckController.OnTriggerEnter())
	{
		if (!player->isDoingFallAttack && attackCooldown.ReadSec() == 0)
			Attack();
		else if (player->isDoingFallAttack)
			Hurt();
	}
}

Enemy::Enemy() : Entity(EntityType::UNKNOWN)
{
}

Enemy::~Enemy()
{
}

bool Enemy::Awake()
{
	return true;
}

bool Enemy::Start()
{
	player = Engine::GetInstance().scene->player;
	texture = Engine::GetInstance().textures->Load(textureName.c_str());
	InitAnimations();
	InitColliders();
	return true;
}

bool Enemy::Update(float dt)
{
	Brain();
	return true;
}

bool Enemy::CleanUp()
{
	return true;
}
