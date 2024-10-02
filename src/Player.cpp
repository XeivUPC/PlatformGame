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

	attackRecoverTimer = Timer();
	jumpRecoverTimer = Timer();

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
	playerCollider = colliderCreator->CreateBox(world, postion, PIXEL_TO_METERS(15), PIXEL_TO_METERS(29));
	playerCollider->SetFixedRotation(true);
	for (b2Fixture* fixture = playerCollider->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
	{
			fixture->SetFriction(0);
	}

	groundCheck = colliderCreator->CreateBox(world, postion, PIXEL_TO_METERS(14), PIXEL_TO_METERS(10));
	groundCheck->SetFixedRotation(true);
	groundCheck->GetFixtureList()[0].SetSensor(true);
	
	b2WeldJointDef weldJointDef;
	weldJointDef.bodyA = playerCollider; 
	weldJointDef.bodyB = groundCheck; 

	weldJointDef.localAnchorA.Set(0.0f, 0.0f); 
	weldJointDef.localAnchorB.Set(0.0f, PIXEL_TO_METERS(-10.5f));

	b2WeldJoint* weldJoint = (b2WeldJoint*)world->CreateJoint(&weldJointDef);


	world->SetContactListener(&groundCheckController);
}


bool Player::Update(float dt)
{

	isGrounded = groundCheckController.IsBeingTriggered();
	if (isGrounded)
		isDoingFallAttack = false;
	if (TryShovelAttack())
		isDoingShovelAttack = false;


	b2Vec2 velocity{ GetMoveInput().x, playerCollider->GetLinearVelocity().y};

	if (Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
		if (TryShovelAttack()) {
			DoShovelAttack();
		}
	}
	if (isDoingShovelAttack && isGrounded)
		velocity.x = 0;


	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		if (TryFallAttack()) {
			DoFallAttack();
		}
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		if (TryJump()) {
			velocity.y = 0;
			playerCollider->SetLinearVelocity(velocity);
			DoJump();
		}
	}

	playerCollider->SetLinearVelocity(velocity);

	float gravityValue = defaultGravity;

	if (isDoingFallAttack)
		gravityValue = fallAttackGravity;
	else if (playerCollider->GetLinearVelocity().y > 0) {
		gravityValue = fallGravity;
	}
	playerCollider->SetGravityScale(gravityValue);

	SetGravityValue(playerCollider->GetLinearVelocity().y);

	if (playerCollider->GetLinearVelocity().y > MAX_FALL_SPEED)
	{
		velocity.y = MAX_FALL_SPEED;
		playerCollider->SetLinearVelocity(velocity);
	}

	position.setX(playerCollider->GetPosition().x);
	position.setY(playerCollider->GetPosition().y);

	if (velocity.x != 0)
	{
		if (velocity.x > 0)
			isFlipped = false;
		else
			isFlipped = true;
	}
	

	Engine::GetInstance().render.get()->DrawTexture(texture, METERS_TO_PIXELS(position.getX()+ textureOffset.x), METERS_TO_PIXELS(position.getY() + textureOffset.y),(SDL_RendererFlip)isFlipped);

	Engine::GetInstance().box2DCreator.get()->RenderBody(playerCollider, b2Color{ 255,0,0,255 });
	Engine::GetInstance().box2DCreator.get()->RenderBody(groundCheck, b2Color{ 0,0,255,255 });

	return true;
}

b2Vec2 Player::GetMoveInput() {
	b2Vec2 velocity{ 0,0 };
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		velocity.x = (-speed);

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		velocity.x = (speed);
	return velocity;

}
bool Player::TryShovelAttack() {
	if (attackRecoverTimer.ReadMSec() >= attackRecoverMS)
		return true;
	return false;
}
void Player::DoShovelAttack() {
	attackRecoverTimer.Start();
	isDoingShovelAttack = true;
}

bool Player::TryFallAttack() {
	if (!isGrounded && !isDoingFallAttack)
		return true;
	return false;
}
void Player::DoFallAttack() {
	isDoingFallAttack = true;
}


bool Player::TryJump() {
	if (isGrounded && !isDoingShovelAttack && jumpRecoverTimer.ReadMSec() >= jumpRecoverMS)
		return true;
	return false;
}

void Player::DoJump() {
	playerCollider->ApplyForceToCenter(b2Vec2{ 0,-jumpForce }, true);
	jumpRecoverTimer.Start();
}

void Player::SetGravityValue(float verticalVelocity) {
	float gravityValue = defaultGravity;

	if (isDoingFallAttack)
		gravityValue = fallAttackGravity;
	else if (verticalVelocity > 0) {
		gravityValue = fallGravity;
	}
	playerCollider->SetGravityScale(gravityValue);
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().scene->world->DestroyBody(playerCollider);
	Engine::GetInstance().scene->world->DestroyBody(groundCheck);
	Engine::GetInstance().textures.get()->UnLoad(texture);
	return true;
}


