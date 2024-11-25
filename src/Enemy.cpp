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

void Enemy::FindCurrentTileInPath()
{
	Vector2D tile = { (float)((int)position.getX()), (float)((int)(position.getY()-0.5f)) };
	currentPathTileIndex = 0;
	for (size_t i = 0; i < pathData.pathTiles.size(); i++)
	{
		if (tile == pathData.pathTiles.at(i))
		{
			currentPathTileIndex = i;
			return;
		}
	}
}

void Enemy::InitAnimations()
{
	texture = Engine::GetInstance().textures->Load(textureName.c_str());
	animator = new Animator();
}

void Enemy::InitColliders()
{
	playerFilter.categoryBits = Engine::GetInstance().ENEMY_ATTACK_LAYER;
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

void Enemy::Move()
{
	float fixedDt = 16 / 1000.0f;
	b2Vec2 bodyDirection = b2Vec2{ enemyDirection.getX()*speed * fixedDt, enemyDirection.getY()*speed* fixedDt };
	enemyCollider->SetLinearVelocity(bodyDirection);
}

void Enemy::Brain()
{
	if (playerDamageCheckController.IsBeingTriggered() && hurtCooldown.ReadMSec() >= hurtCooldownMS)
	{
		hurtCooldown.Start();
		Hurt();
	}
	else if (playerCheckController.IsBeingTriggered() && attackCooldown.ReadMSec() >= attackCooldownMS)
	{
		attackCooldown.Start();
		Attack();
	}

	if (enemyDirection.getY() != 0)enemyCollider->SetGravityScale(0);
	else enemyCollider->SetGravityScale(1);
}

void Enemy::Render(float dt)
{
	animator->Update(dt);
	Engine::GetInstance().render->SelectLayer(Render::RenderLayers::Enemy);
	if(enemyDirection.getX()>0)
		animator->Animate(METERS_TO_PIXELS(position.getX()) + textureOffset.getX(), METERS_TO_PIXELS(position.getY()) + textureOffset.getY(), SDL_FLIP_NONE);
	else
		animator->Animate(METERS_TO_PIXELS(position.getX()) + textureOffset.getX(), METERS_TO_PIXELS(position.getY()) + textureOffset.getY(), SDL_FLIP_HORIZONTAL);
	if (pathData.pathTiles.size() > 0)
	{
		Engine::GetInstance().render->LockLayer(Render::Layer7);
		Engine::GetInstance().render->DrawCircle(METERS_TO_PIXELS(pathData.pathTiles.at(currentPathTileIndex).getX() + 0.5f), METERS_TO_PIXELS(pathData.pathTiles.at(currentPathTileIndex).getY()+1.5f), 8, 255, 255, 255);
		Engine::GetInstance().render->UnlockLayer();
	}

	if (Engine::GetInstance().debug->HasDebug(1))
	{
		Engine::GetInstance().render->LockLayer(Render::RenderLayers::Enemy);
		Box2DRender::GetInstance().RenderBody(enemyCollider, { 255,0,0,255 });
		Engine::GetInstance().render->UnlockLayer();
	}
}

void Enemy::SetPathDirection()
{
	enemyDirection = { 0,0 };
	if (pathData.pathTiles.size() == 0)
	{

	}
	if (currentPathTileIndex - 1 > 0 && currentPathTileIndex< pathData.pathTiles.size()) {
		enemyDirection = Vector2D{pathData.pathTiles.at(currentPathTileIndex-1).getX() - pathData.pathTiles.at(currentPathTileIndex).getX(), pathData.pathTiles.at(currentPathTileIndex-1).getY() - pathData.pathTiles.at(currentPathTileIndex).getY() };
	}
	if (enemyDirection.getX() == 0 && enemyDirection.getY() ==0) {
		Vector2D playerPosition = Engine::GetInstance().scene->player->position;
		if (playerPosition.getX() > position.getX()) {
			enemyDirection.setX(1);
		}else
			enemyDirection.setX(-1);
	}
}

Enemy::Enemy(Vector2D pos, MapLayer* pathLayer) : Entity(EntityType::UNKNOWN)
{
	position = pos;
	mapData = pathLayer;
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
