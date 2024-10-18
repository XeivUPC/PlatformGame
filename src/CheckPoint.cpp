#include "CheckPoint.h"
#include "Engine.h"
#include "Physics.h"
#include "Audio.h"
#include "Textures.h"
#include "Box2DFactory.h"
#include "Box2DRender.h"
#include "CollisionsManager.h"
#include "LevelManager.h"

CheckPoint::CheckPoint(int sectionPlaced, Vector2D respawnPoint) : Entity(EntityType::UNKNOWN)
{
	this->sectionPlaced = sectionPlaced;
	respawnPos = respawnPoint;
	position = respawnPos;

	Engine::GetInstance().levelManager->RegisterCheckPoint(this);
}

CheckPoint::~CheckPoint()
{

}

bool CheckPoint::Awake()
{
	return true;
}

bool CheckPoint::Start()
{
	animator = new Animator();
	turnOnTimer = Timer();
	unlockSoundId = Engine::GetInstance().audio->LoadFx("CheckPoint_Unlock.wav");
	texture = Engine::GetInstance().textures->Load("Assets/Textures/Objects/CheckPoint.png");

	AnimationData turnedOff = AnimationData("Off");
	turnedOff.AddSprite(Sprite{ texture,{0.0f, 0.0f}, {64, 64} });

	AnimationData turningOn = AnimationData("TurningOn");
	turningOn.AddSprite(Sprite{ texture,{0.0f, 1.0f}, {64, 64} });
	turningOn.AddSprite(Sprite{ texture,{1.0f, 1.0f}, {64, 64} });
	turningOn.AddSprite(Sprite{ texture,{2.0f, 1.0f}, {64, 64} });
	turningOn.AddSprite(Sprite{ texture,{3.0f, 1.0f}, {64, 64} });

	AnimationData turnedOn = AnimationData("On");
	turnedOn.AddSprite(Sprite{ texture,{0.0f, 2.0f}, {64, 64} });
	turnedOn.AddSprite(Sprite{ texture,{1.0f, 2.0f}, {64, 64} });
	turnedOn.AddSprite(Sprite{ texture,{2.0f, 2.0f}, {64, 64} });

	
	animator->AddAnimation(turnedOff);
	animator->AddAnimation(turningOn);
	animator->AddAnimation(turnedOn);

	animator->SelectAnimation("On", true);

	animator->SetSpeed(100);

	b2Filter filter;
	filter.categoryBits = Engine::GetInstance().INTERACTABLE_LAYER;
	filter.maskBits = Engine::GetInstance().PLAYER_LAYER;


	Box2DFactory& colliderCreator = Box2DFactory::GetInstance();
	b2World* world = Engine::GetInstance().physics->world;
	b2Vec2 colliderPosition{ (position.getX()), (position.getY()-2.5f)};

	b2FixtureUserData fixtureData;
	fixtureData.pointer = (uintptr_t)(&collisionController);
	body = colliderCreator.CreateBox(world, colliderPosition, PIXEL_TO_METERS(25), PIXEL_TO_METERS(25), fixtureData);
	body->GetFixtureList()[0].SetFilterData(filter);
	body->GetFixtureList()[0].SetSensor(true);
	body->SetType(b2_staticBody);

	collisionController.SetBodyToTrack(&body->GetFixtureList()[0]);

	return true;
}

bool CheckPoint::Update(float dt)
{

	if (!isUnlocked && collisionController.IsBeingTriggered())
		Unlock();

	if (isUnlocked) {
		if (turnOnTimer.ReadMSec() <= turnOnMS) {
			animator->SelectAnimation("TurningOn", true);
			
		}
		else {
			animator->SelectAnimation("On", true);
		}
	}
	else {
		animator->SelectAnimation("Off", true);
	}
	
	Engine::GetInstance().render->SelectLayer(Render::RenderLayers::Layer2);
	animator->Update(dt);
	animator->Animate(METERS_TO_PIXELS(position.getX()) + textureOffset.getX(), METERS_TO_PIXELS(position.getY()) + textureOffset.getY(), SDL_FLIP_NONE);

	//Engine::GetInstance().render->SelectLayer(Render::RenderLayers::Layer7);
	//Box2DRender::GetInstance().RenderBody(body, {0,255,0,255});
	return true;
}

bool CheckPoint::CleanUp()
{
	delete animator;
	Engine::GetInstance().physics->world->DestroyBody(body);
	//Engine::GetInstance().textures->UnLoad(texture);
	return true;
}

void CheckPoint::Unlock()
{
	
	Engine::GetInstance().audio->PlayFx(unlockSoundId);
	isUnlocked = true;
	turnOnTimer.Start();
}

bool CheckPoint::IsUnlocked()
{
	return isUnlocked;
}

int CheckPoint::GetSection()
{
	return sectionPlaced;
}

Vector2D CheckPoint::GetRespawnPos()
{
	return respawnPos;
}
