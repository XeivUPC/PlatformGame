#include "Enemy.h"
#include "Engine.h"
#include "Physics.h"
#include "Scene.h"
#include "EntityManager.h"
#include "box2DCreator.h"
#include "Textures.h"
#include "Window.h"
#include "Log.h"


void Enemy::InitAnimations()
{
	texture = Engine::GetInstance().textures->Load(textureName.c_str());
	animator = new Animator();
}

void Enemy::InitColliders()
{
	playerFilter.categoryBits = Engine::GetInstance().ENEMY_LAYER;
	playerFilter.maskBits = Engine::GetInstance().PLAYER_LAYER;
	playerFilter.maskBits = Engine::GetInstance().PLAYER_ATTACK_LAYER;

	groundFilter.categoryBits = Engine::GetInstance().ENEMY_LAYER;
	groundFilter.maskBits = Engine::GetInstance().GROUND_LAYER;
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

Enemy::Enemy(Vector2D pos) : Entity(EntityType::UNKNOWN)
{
	position = pos;
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
	InitAnimations();
	InitColliders();
	return true;
}

bool Enemy::Update(float dt)
{
	Brain();

	position.setX(enemyCollider->GetPosition().x);
	position.setY(enemyCollider->GetPosition().y);

	animator->Update(dt);
	Engine::GetInstance().render->SelectLayer(6);
	animator->Animate(METERS_TO_PIXELS(position.getX()+textureOffset.getX()), METERS_TO_PIXELS(position.getY() + textureOffset.getY()), SDL_FLIP_NONE);
	Engine::GetInstance().render->SelectLayer(6);
	Engine::GetInstance().box2DCreator->RenderBody(enemyCollider, b2Color{ 255,0,0,255 });
	return true;
}

bool Enemy::CleanUp()
{
	LOG("Cleanup enemy");
	delete animator;
	Engine::GetInstance().physics->world->DestroyBody(enemyCollider);
	Engine::GetInstance().textures->UnLoad(texture);
	return true;
}
