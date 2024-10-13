#include "DirtBlock.h"
#include "Engine.h"
#include "Scene.h"
#include "Audio.h"
#include "Textures.h"
#include "Box2DCreator.h"
#include "CollidersManager.h"
#include "EntityManager.h"

DirtBlock::DirtBlock(DirtSize type, Vector2D position) : Entity(EntityType::UNKNOWN)
{
	this->type = type;
	this->position = position;
}

DirtBlock::~DirtBlock()
{
	
}

bool DirtBlock::Awake()
{
	return true;
}

bool DirtBlock::Start()
{
	particleRemoveTimer = Timer();
	breakSoundId = Engine::GetInstance().audio->LoadFx("DirtBlock_Break.wav");
	animator = new Animator();

	Vector2D blockSizeTile;
	Vector2D particleSizeTile;

	if (type == Small) {
		position.setX(position.getX() + 0.5f);
		position.setY(position.getY() + 0.5f);
		texture = Engine::GetInstance().textures->Load("Assets/Textures/Objects/DirtBlock_Small.png");

		blockSizeTile = { 16,16 };
		particleSizeTile = { 24,18 };

		blockTextureOffset = { -8,-8 };
		particleTextureOffset = { -12,-9 };
	}
	else if (type == Big) {
		position.setX(position.getX() + 1.0f);
		position.setY(position.getY() + 1.0f);
		texture = Engine::GetInstance().textures->Load("Assets/Textures/Objects/DirtBlock_Big.png");

		blockSizeTile = { 32,32 };
		particleSizeTile = { 48,36 };

		blockTextureOffset = { -16,-16 };
		particleTextureOffset = { -24,-18 };
	}

	b2Filter filter;
	filter.categoryBits = Engine::GetInstance().GROUND_LAYER;
	filter.maskBits = Engine::GetInstance().PLAYER_LAYER;


	const std::shared_ptr<Box2DCreator>& colliderCreator = Engine::GetInstance().box2DCreator;
	b2World* world = Engine::GetInstance().scene->world;
	b2Vec2 colliderPosition{ (position.getX()), (position.getY()) };

	body = colliderCreator->CreateBox(world, colliderPosition, PIXEL_TO_METERS(blockSizeTile.getX()), PIXEL_TO_METERS(blockSizeTile.getY()));
	body->GetFixtureList()[0].SetFilterData(filter);
	body->SetType(b2_staticBody);

	filter.categoryBits = Engine::GetInstance().ENEMY_LAYER;
	filter.maskBits = Engine::GetInstance().PLAYER_ATTACK_LAYER;
	hitCheck = colliderCreator->AddBox(body, { 0,0}, PIXEL_TO_METERS(blockSizeTile.getX()), PIXEL_TO_METERS(blockSizeTile.getY())+0.7f);
	hitCheck->SetSensor(true);
	hitCheck->SetFilterData(filter);

	Engine::GetInstance().box2DSensors->AddSensor(&collisionController);
	collisionController.SetSensor(&body->GetFixtureList()[0]);




	AnimationData defaultBlock = AnimationData("Default");
	defaultBlock.AddSprite(Sprite{ texture,{0.0f, 0.0f}, {blockSizeTile.getX(), blockSizeTile.getY()}});

	AnimationData particleAnim = AnimationData("Particle");
	particleAnim.AddSprite(Sprite{ texture,{0.0f, 1.0f}, {particleSizeTile.getX(), particleSizeTile.getY()} });
	particleAnim.AddSprite(Sprite{ texture,{1.0f, 1.0f}, {particleSizeTile.getX(), particleSizeTile.getY()} });
	particleAnim.AddSprite(Sprite{ texture,{2.0f, 1.0f}, {particleSizeTile.getX(), particleSizeTile.getY()} });

	animator->AddAnimation(defaultBlock);
	animator->AddAnimation(particleAnim);

	animator->SelectAnimation("Default", true);

	animator->SetSpeed(100);

	return true;

}

bool DirtBlock::Update(float dt)
{
	if (!isBroken) {
		if (collisionController.IsBeingTriggered()) {
			Break();
		}
	}

	if (isBroken && particleRemoveTimer.ReadMSec() >= particleRemoveMS) {
		Engine::GetInstance().entityManager->DestroyEntityAtUpdateEnd(this);
		return true;
	}

	Vector2D textureOffset = blockTextureOffset;
	if (isBroken) {
		animator->SelectAnimation("Particle", false);
		textureOffset = particleTextureOffset;

	}
	else {
		animator->SelectAnimation("Default", true);
	}

	
	animator->Update(dt);
	animator->Animate(METERS_TO_PIXELS(position.getX()) + textureOffset.getX(), METERS_TO_PIXELS(position.getY()) + textureOffset.getY(), SDL_FLIP_NONE);

	if(!isBroken)
		Engine::GetInstance().box2DCreator->RenderBody(body, { 0,255,0,255 });

	return true;
}

bool DirtBlock::CleanUp()
{
	delete animator;
	return true;
}

void DirtBlock::Break()
{
	isBroken = true;
	particleRemoveTimer.Start();
	Engine::GetInstance().scene->world->DestroyBody(body);
	Engine::GetInstance().audio->PlayFx(breakSoundId);

}
