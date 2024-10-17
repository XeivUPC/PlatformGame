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
	playerFilter.maskBits = Engine::GetInstance().PLAYER_LAYER | Engine::GetInstance().PLAYER_ATTACK_LAYER;

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
	if (playerCheckController.IsBeingTriggered())
	{
		if ((!player->isDoingShovelAttack && !player->isDoingFallAttack) && attackCooldown.ReadMSec() >= attackCooldownMS)
		{
			attackCooldown.Start();
			Attack();
		}
		else if ((player->isDoingShovelAttack || player->isDoingFallAttack) && hurtCooldown.ReadMSec() >= hurtCooldownMS)
		{
			hurtCooldown.Start();
			Hurt();
			printf("Ouch\n");
		}
	}
}

void Enemy::Render(float dt)
{
	animator->Update(dt);
	Engine::GetInstance().render->SelectLayer(9);
	animator->Animate(METERS_TO_PIXELS(position.getX() + textureOffset.getX()), METERS_TO_PIXELS(position.getY() + textureOffset.getY()), SDL_FLIP_NONE);
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
	position.setX(enemyCollider->GetPosition().x);
	position.setY(enemyCollider->GetPosition().y);
	Brain();
	Render(dt);
	//Engine::GetInstance().box2DCreator->RenderBody(enemyCollider, b2Color{ 255,0,0,255 });
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
