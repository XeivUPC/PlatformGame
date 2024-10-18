#include "Enemy.h"
#include "Engine.h"
#include "Physics.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Box2DFactory.h"
#include "Textures.h"
#include "Window.h"
#include "Log.h"
#include "Debug.h"
#include "Box2DRender.h"



void Enemy::InitAnimations()
{
	texture = Engine::GetInstance().textures->Load(textureName.c_str());
	animator = new Animator();
}

void Enemy::InitColliders()
{
	playerFilter.categoryBits = Engine::GetInstance().ENEMY_LAYER;
	playerFilter.maskBits = Engine::GetInstance().PLAYER_LAYER;
	
	playerDamageFilter.categoryBits = Engine::GetInstance().ENEMY_LAYER;
	playerDamageFilter.maskBits = Engine::GetInstance().PLAYER_ATTACK_LAYER;

	groundFilter.categoryBits = Engine::GetInstance().ENEMY_LAYER;
	groundFilter.maskBits = Engine::GetInstance().GROUND_LAYER;
	
}

void Enemy::Attack()
{
	player->Damage(hitDamage, {0,1});
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
	b2Vec2 bodyDirection = b2Vec2{ enemyDirection.getX()*speed, enemyDirection.getY()*speed };
	enemyCollider->SetLinearVelocity(bodyDirection);
}

Vector2D Enemy::TrackPlayerPosition(bool verticalAxis, bool horizontalAxis)
{
	Vector2D playerPosition = Engine::GetInstance().scene->player->position;
	playerPosition.setX(playerPosition.getX() - position.getX());
	playerPosition.setY(playerPosition.getY() - position.getY());
	if (!verticalAxis)
		playerPosition.setY(0);
	if (!verticalAxis)
		playerPosition.setX(0);
	if(playerPosition.getX() < 0)

	return playerPosition;
}

void Enemy::Brain()
{
	if (playerDamageCheckController.IsBeingTriggered() && hurtCooldown.ReadMSec() >= hurtCooldownMS)
	{
		hurtCooldown.Start();
		Hurt();
	}
	if (playerCheckController.IsBeingTriggered() && attackCooldown.ReadMSec() >= attackCooldownMS)
	{
		attackCooldown.Start();
		Attack();
	}
	Move();
}

void Enemy::Render(float dt)
{
	animator->Update(dt);
	Engine::GetInstance().render->SelectLayer(Render::RenderLayers::Enemy);
	if(enemyDirection.getY()>0)
		animator->Animate(METERS_TO_PIXELS(position.getX()) + textureOffset.getX(), METERS_TO_PIXELS(position.getY()) + textureOffset.getY(), SDL_FLIP_NONE);
	else
		animator->Animate(METERS_TO_PIXELS(position.getX()) + textureOffset.getX(), METERS_TO_PIXELS(position.getY()) + textureOffset.getY(), SDL_FLIP_HORIZONTAL);
	if (Engine::GetInstance().debug->HasDebug(1))
	{
		Engine::GetInstance().render->LockLayer(Render::RenderLayers::Enemy);
		Box2DRender::GetInstance().RenderBody(enemyCollider, { 255,0,0,255 });
		Engine::GetInstance().render->UnlockLayer();
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
	position.setX(enemyCollider->GetPosition().x);
	position.setY(enemyCollider->GetPosition().y);
	Brain();
	Render(dt);
	return true;
}

bool Enemy::CleanUp()
{
	LOG("Cleanup enemy");
	delete animator;
	Engine::GetInstance().physics->world->DestroyBody(enemyCollider);
	//Engine::GetInstance().textures->UnLoad(texture);
	return true;
}
