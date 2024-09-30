#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name = "Player";
}

Player::~Player() {

}

bool Player::Awake() {

	//Initialize Player parameters
	position = Vector2D(0, 0);


	b2BodyDef boxBodyDef;
	b2MassData* boxMassData = new b2MassData();
	boxMassData->mass = 100;

	boxBodyDef.type = b2_dynamicBody;
	boxBodyDef.position.Set(PIXEL_TO_METERS(position.getX()), PIXEL_TO_METERS(position.getY()));
	body = Engine::GetInstance().scene->world->CreateBody(&boxBodyDef);
	body->SetMassData(boxMassData);

	b2CircleShape boxShape;
	boxShape.m_radius = PIXEL_TO_METERS(10);

	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &boxShape;
	boxFixtureDef.density = 1.0f;
	boxFixtureDef.friction = 0;
	body->CreateFixture(&boxFixtureDef);

	return true;
}

bool Player::Start() {

	texture = Engine::GetInstance().textures.get()->Load(textureName.c_str());
	return true;
}

bool Player::Update(float dt)
{
	//Render the player texture and modify the position of the player using WSAD keys and render the texture
	b2Vec2 velocity{ 0, body->GetLinearVelocity().y};

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		velocity.x =(-speed);

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		velocity.x =(speed);

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		velocity.y = 0;
		body->SetLinearVelocity(velocity);
		body->ApplyForceToCenter(b2Vec2{ 0,-25 }, true);
	}
	else
		body->SetLinearVelocity(velocity);

	if (body->GetLinearVelocity().y > 0) {
		body->SetGravityScale(2.0f);
	}
	else {
		body->SetGravityScale(1);
	}




	position.setX(body->GetPosition().x);
	position.setY(body->GetPosition().y);

	Engine::GetInstance().render.get()->DrawTexture(texture, METERS_TO_PIXELS(position.getX()), METERS_TO_PIXELS(position.getY()));
	return true;
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().scene->world->DestroyBody(body);
	Engine::GetInstance().textures.get()->UnLoad(texture);
	return true;
}