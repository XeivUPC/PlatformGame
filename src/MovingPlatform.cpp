#include "MovingPlatform.h"
#include "Engine.h"
#include "Physics.h"
#include "Audio.h"
#include <set>
#include "Textures.h"
#include "Box2DFactory.h"
#include "Box2DRender.h"
#include "CollisionsManager.h"
#include "LevelManager.h"
#include "Debug.h"

MovingPlatform::MovingPlatform(int sectionPlaced, Vector2D leftPoint, Vector2D rightPoint, int platformType, bool verticalMovement) : Entity(EntityType::UNKNOWN)
{
	this->sectionPlaced = sectionPlaced;
	this->leftPoint = leftPoint;
	this->rightPoint = rightPoint;
	this->platformType = platformType;
	isVertical = verticalMovement;

	position = this->leftPoint;
}

MovingPlatform::~MovingPlatform()
{
}

bool MovingPlatform::Awake()
{
	return true;
}

bool MovingPlatform::Start()
{
	animator = new Animator();
	texture = Engine::GetInstance().textures->Load("Assets/Textures/Objects/Moving_Platforms.png");

	AnimationData moving = AnimationData("Moving");
	moving.AddSprite(Sprite{ texture,{0.0f, (float)platformType}, {48, 16} });

	animator->AddAnimation(moving);
	animator->SelectAnimation("Moving", true);

	animator->SetSpeed(100);

	b2Filter filter;
	filter.categoryBits = Engine::GetInstance().GROUND_LAYER;
	filter.maskBits = Engine::GetInstance().PLAYER_LAYER;


	Box2DFactory& colliderCreator = Box2DFactory::GetInstance();
	b2World* world = Engine::GetInstance().physics->world;
	b2Vec2 colliderPosition{ (position.getX()), (position.getY()) };

	b2FixtureUserData fixtureData;
	fixtureData.pointer = (uintptr_t)(&collisionController);
	body = colliderCreator.CreateBox(world, colliderPosition, PIXEL_TO_METERS(16*3), PIXEL_TO_METERS(16), fixtureData);
	body->GetFixtureList()[0].SetFilterData(filter);
	body->SetType(b2_kinematicBody);
	body->SetGravityScale(0);
	body->SetFixedRotation(true);

	collisionController.SetBodyToTrack(&body->GetFixtureList()[0]);

	return true;
}

bool MovingPlatform::Update(float dt)
{
	if (movingBackwards) {
		if(isVertical)
			body->SetLinearVelocity({ 0,-speed * dt / 1000 });
		else
			body->SetLinearVelocity({ -speed * dt / 1000,0 });
	}
	else {
		if (isVertical)
			body->SetLinearVelocity({ 0,+speed * dt / 1000 });
		else
			body->SetLinearVelocity({ +speed * dt / 1000,0 });
	}

	if (isVertical) {
		if (body->GetPosition().y <= rightPoint.getY())
		{
			movingBackwards = false;
		}

		if (body->GetPosition().y >= leftPoint.getY())
		{
			movingBackwards = true;
		}
	}
	else {
		if (body->GetPosition().x <= leftPoint.getX())
		{
			movingBackwards = false;
		}

		if (body->GetPosition().x >= rightPoint.getX())
		{
			movingBackwards = true;
		}
	}



	position.setX(body->GetPosition().x);
	position.setY(body->GetPosition().y);

	Engine::GetInstance().render->SelectLayer(Render::RenderLayers::Layer2);
	animator->Update(dt);
	animator->Animate(METERS_TO_PIXELS(position.getX()) + textureOffset.getX(), METERS_TO_PIXELS(position.getY()) + textureOffset.getY(), SDL_FLIP_NONE);

	if (Engine::GetInstance().debug->HasDebug(1))
	{
		Engine::GetInstance().render->LockLayer(Render::RenderLayers::Layer7);
		if (isVertical) {
			Engine::GetInstance().render->DrawLine(METERS_TO_PIXELS(leftPoint.getX()), METERS_TO_PIXELS(leftPoint.getY()), METERS_TO_PIXELS(rightPoint.getX()), METERS_TO_PIXELS(rightPoint.getY()), 255, 255, 255, 255, true);
		}
		else {
			Engine::GetInstance().render->DrawLine(METERS_TO_PIXELS(leftPoint.getX()), METERS_TO_PIXELS(leftPoint.getY()), METERS_TO_PIXELS(rightPoint.getX()), METERS_TO_PIXELS(rightPoint.getY()), 255, 255, 255, 255, true);
		}

		Box2DRender::GetInstance().RenderBody(body, { 255,255,0,255 });
		Engine::GetInstance().render->UnlockLayer();
	}

	if (!isVertical)
	{
		b2Vec2 platVel = body->GetLinearVelocity();
		for (b2Body* entity : collisionController.GetBodiesColliding())
		{
			b2Vec2 vel = entity->GetLinearVelocity();
			entity->SetLinearVelocity({ vel.x + platVel.x, vel.y + platVel.y });
		}
	}

	return true;
}


bool MovingPlatform::CleanUp()
{
	delete animator;
	Engine::GetInstance().physics->world->DestroyBody(body);
	//Engine::GetInstance().textures->UnLoad(texture);
	return true;
}

int MovingPlatform::GetSection()
{
	return sectionPlaced;
}
