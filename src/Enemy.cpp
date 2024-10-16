#include "Enemy.h"
#include "Engine.h"
#include "Physics.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Textures.h"
#include "Window.h"

void Enemy::InitAnimations()
{
	texture = Engine::GetInstance().textures->Load(textureName.c_str());
}

void Enemy::InitColliders()
{
	enemyCollider->ResetMassData();

	b2MassData massData;
	massData.mass = enemyMass;
	massData.center = enemyCollider->GetLocalCenter();
	enemyCollider->SetMassData(&massData);

	playerFilter.categoryBits = Engine::GetInstance().ENEMY_LAYER;
	playerFilter.maskBits = Engine::GetInstance().PLAYER_LAYER;
	playerFilter.maskBits = Engine::GetInstance().PLAYER_ATTACK_LAYER;

	groundFilter.categoryBits = Engine::GetInstance().ENEMY_LAYER;
	groundFilter.maskBits = Engine::GetInstance().GROUND_LAYER;
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
