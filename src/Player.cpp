#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Box2DCreator.h"
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

	InitColliders();
	groundCheckController.SetSensor(groundCheck);

	return true;
}

bool Player::Start() {

	texture = Engine::GetInstance().textures.get()->Load(textureName.c_str());
	return true;
}

void Player::InitColliders() {

	Box2DCreator* colliderCreator = Engine::GetInstance().box2DCreator.get();

	b2World* world = Engine::GetInstance().scene.get()->world;
	b2Vec2 postion{ PIXEL_TO_METERS(position.getX()), PIXEL_TO_METERS(position.getY()) };
	playerCollider = colliderCreator->CreateCapsule(world, postion, PIXEL_TO_METERS(10), PIXEL_TO_METERS(30), PIXEL_TO_METERS(5.5f));
	playerCollider->SetFixedRotation(true);

	groundCheck = colliderCreator->CreateBox(world, postion, PIXEL_TO_METERS(15), PIXEL_TO_METERS(10));
	groundCheck->SetFixedRotation(true);
	groundCheck->GetFixtureList()[0].SetSensor(true);
	
	b2WeldJointDef weldJointDef;
	weldJointDef.bodyA = playerCollider;  // Primer cuerpo
	weldJointDef.bodyB = groundCheck;  // Segundo cuerpo

	// Punto de anclaje donde los cuerpos estarán unidos
	// Puedes usar el centro de uno de los cuerpos, por ejemplo:
	weldJointDef.localAnchorA.Set(0.0f, 0.0f);  // Centro de body1
	weldJointDef.localAnchorB.Set(0.0f, PIXEL_TO_METERS(-10));  // Centro de body2

	// Crear el joint en el mundo de Box2D
	b2WeldJoint* weldJoint = (b2WeldJoint*)world->CreateJoint(&weldJointDef);


	world->SetContactListener(&groundCheckController);
}




bool Player::Update(float dt)
{

	isGrounded = groundCheckController.IsBeingTriggered();

	//Render the player texture and modify the position of the player using WSAD keys and render the texture
	b2Vec2 velocity{ 0, playerCollider->GetLinearVelocity().y};

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		velocity.x =(-speed);

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		velocity.x =(speed);

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isGrounded) {
		velocity.y = 0;
		playerCollider->SetLinearVelocity(velocity);
		playerCollider->ApplyForceToCenter(b2Vec2{ 0,-45 }, true);
	}
	else
		playerCollider->SetLinearVelocity(velocity);

	if (playerCollider->GetLinearVelocity().y > 0) {
		playerCollider->SetGravityScale(2.5f);
	}
	else {
		playerCollider->SetGravityScale(1);
	}

	if (velocity.x != 0)
	{
		if (velocity.x > 0)
			isFlipped = false;
		else
			isFlipped = true;
	}


	position.setX(playerCollider->GetPosition().x);
	position.setY(playerCollider->GetPosition().y);



	Engine::GetInstance().render.get()->DrawTexture(texture, METERS_TO_PIXELS(position.getX()+ textureOffset.x), METERS_TO_PIXELS(position.getY() + textureOffset.y),(SDL_RendererFlip)isFlipped);

	Engine::GetInstance().box2DCreator.get()->RenderBody(playerCollider, b2Color{ 255,0,0,255 });
	Engine::GetInstance().box2DCreator.get()->RenderBody(groundCheck, b2Color{ 0,0,255,255 });

	return true;
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().scene->world->DestroyBody(playerCollider);
	Engine::GetInstance().scene->world->DestroyBody(groundCheck);
	Engine::GetInstance().textures.get()->UnLoad(texture);
	return true;
}


