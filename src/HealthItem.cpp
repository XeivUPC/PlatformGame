#include "HealthItem.h"
#include "Scene.h"
#include "Engine.h"
#include "Box2DFactory.h"
#include "Debug.h"
#include "Box2DRender.h"
#include "Textures.h"

Healthtem::Healthtem(Vector2D position, int id) : Item(position, id)
{
}

Healthtem::~Healthtem()
{
}

bool Healthtem::Awake()
{
	return true;
}

bool Healthtem::Start()
{
	animator = new Animator();
	texture = Engine::GetInstance().textures->Load("Assets/Textures/Objects/HealthPotion.png");
	textureOffset = { -6,-9.5 };

	AnimationData defaultAnim = AnimationData("Default");
	defaultAnim.AddSprite(Sprite{ texture,{0.0f, 0.0f}, {13, 19} });
	defaultAnim.AddSprite(Sprite{ texture,{1.0f, 0.0f}, {13, 19} });
	defaultAnim.AddSprite(Sprite{ texture,{2.0f, 0.0f}, {13, 19} });
	defaultAnim.AddSprite(Sprite{ texture,{3.0f, 0.0f}, {13, 19} });

	animator->AddAnimation(defaultAnim);
	animator->SelectAnimation("Default", true);
	animator->SetSpeed(120);

	///Create Body
	Box2DFactory& colliderCreator = Box2DFactory::GetInstance();
	b2World* world = Engine::GetInstance().physics->world;

	b2FixtureUserData fixtureData;
	fixtureData.pointer = (uintptr_t)(&collisionController);

	position.setY(position.getY() + PIXEL_TO_METERS(7));
	body = colliderCreator.CreateBox(world, { position.getX(), position.getY()}, PIXEL_TO_METERS(13), PIXEL_TO_METERS(14), fixtureData);
	Item::Start();
	return true;
}

bool Healthtem::Update(float dt)
{
	//Render
	Engine::GetInstance().render->SelectLayer(Render::RenderLayers::Layer2);
	animator->Update(dt);
	animator->Animate(METERS_TO_PIXELS(position.getX()) + textureOffset.getX(), METERS_TO_PIXELS(position.getY()) + textureOffset.getY(), SDL_FLIP_NONE);

	if (Engine::GetInstance().debug->HasDebug(1))
	{
		Engine::GetInstance().render->LockLayer(Render::RenderLayers::Layer7);
		Box2DRender::GetInstance().RenderBody(body, { 255,0,0,255 });
		Engine::GetInstance().render->UnlockLayer();
	}

	Item::Update(dt);


	return true;
}

bool Healthtem::CleanUp()
{
	delete animator;
	Item::CleanUp();
	return true;
}


void Healthtem::Pick()
{
	Engine::GetInstance().scene->player->playerHealth.Heal(2);
	Item::Pick();
}
