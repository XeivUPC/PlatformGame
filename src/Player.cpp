#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Box2DCreator.h"
#include "CollidersManager.h"
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
	position = Vector2D(16, 16*8);

	InitColliders();
	groundCheckController.SetSensor(groundCheck);
	enemyCheckController.SetSensor(enemyCheck);

	attackRecoverTimer = Timer();
	jumpRecoverTimer = Timer();

	/// Texture, index, size, pivot

	


	return true;
}

bool Player::Start() {

	texture = Engine::GetInstance().textures.get()->Load(textureName.c_str());

	InitAnimations();

	return true;
}

void Player::InitAnimations() {


	AnimationData idle = AnimationData("Player_Idle");
	idle.AddSprite(Sprite{ texture,{0.0f, 0.0f}, {70, 70}});

	AnimationData idle2 = AnimationData("Player_Idle_2");
	idle2.AddSprite(Sprite{ texture,{1.0f, 0.0f}, {70, 70}});
	idle2.AddSprite(Sprite{ texture,{2.0f, 0.0f}, {70, 70}});
	idle2.AddSprite(Sprite{ texture,{3.0f, 0.0f}, {70, 70}});

	AnimationData move = AnimationData("Player_Move");
	move.AddSprite(Sprite{ texture,{0.0f, 1.0f}, {70, 70}});
	move.AddSprite(Sprite{ texture,{1.0f, 1.0f}, {70, 70}});
	move.AddSprite(Sprite{ texture,{2.0f, 1.0f}, {70, 70}});
	move.AddSprite(Sprite{ texture,{3.0f, 1.0f}, {70, 70}});
	move.AddSprite(Sprite{ texture,{4.0f, 1.0f}, {70, 70}});
	move.AddSprite(Sprite{ texture,{5.0f, 1.0f}, {70, 70}});

	AnimationData jump_rise = AnimationData("Player_Jump_Rise");
	jump_rise.AddSprite(Sprite{ texture,{0.0f, 2.0f}, {70, 70}});

	AnimationData jump_fall = AnimationData("Player_Jump_Fall");
	jump_fall.AddSprite(Sprite{ texture,{0.0f, 3.0f}, {70, 70}});

	AnimationData fall_attack = AnimationData("Player_Fall_Attack");
	fall_attack.AddSprite(Sprite{ texture,{0.0f, 4.0f}, {70, 70}});

	AnimationData attack = AnimationData("Player_Attack");
	attack.AddSprite(Sprite{ texture,{0.0f, 5.0f}, {70, 70} });
	attack.AddSprite(Sprite{ texture,{1.0f, 5.0f}, {70, 70}});
	attack.AddSprite(Sprite{ texture,{2.0f, 5.0f}, {70, 70}});
	attack.AddSprite(Sprite{ texture,{3.0f, 5.0f}, {70, 70}});


	animator.AddAnimation(idle);
	animator.AddAnimation(idle2);
	animator.AddAnimation(move);
	animator.AddAnimation(jump_rise);
	animator.AddAnimation(jump_fall);
	animator.AddAnimation(fall_attack);
	animator.AddAnimation(attack);
	animator.SelectAnimation("Player_Idle", true);

	animator.SetSpeed(100);
}

void Player::InitColliders() {

	Box2DCreator* colliderCreator = Engine::GetInstance().box2DCreator.get();
	b2World* world = Engine::GetInstance().scene.get()->world;

	///PlayerCollider

	b2Vec2 playerColliderPosition{ PIXEL_TO_METERS(position.getX()), PIXEL_TO_METERS(position.getY()) };

	playerFilters.categoryBits = Engine::GetInstance().PLAYER_LAYER;
	playerFilters.maskBits = Engine::GetInstance().GROUND_LAYER;

	enemyCheckFilters.categoryBits = Engine::GetInstance().PLAYER_ATTACK_LAYER;
	enemyCheckFilters.maskBits = Engine::GetInstance().ENEMY_LAYER;

	emptyFilter.maskBits = 0x0000;

	playerCollider = colliderCreator->CreateBox(world, playerColliderPosition, PIXEL_TO_METERS(15), PIXEL_TO_METERS(29));
	playerCollider->SetFixedRotation(true);
	for (b2Fixture* fixture = playerCollider->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
	{
		fixture->SetFriction(0);
		fixture->SetFilterData(playerFilters);
	}

	groundCheck = colliderCreator->AddBox(playerCollider, b2Vec2(0.0f, PIXEL_TO_METERS(10.5f)), PIXEL_TO_METERS(14), PIXEL_TO_METERS(10));
	groundCheck->SetSensor(true);
	groundCheck->SetFilterData(playerFilters);


	enemyCheck = colliderCreator->AddBox(playerCollider, b2Vec2(0.0f, PIXEL_TO_METERS(16.5f)), PIXEL_TO_METERS(14), PIXEL_TO_METERS(2));
	//enemyCheck->SetSensor(true)
	enemyCheck->SetFilterData(enemyCheckFilters);
	enemyCheck->SetFriction(0);
	enemyCheckController.AcceptOnlyTriggers(false);


	Engine::GetInstance().box2DSensors.get()->AddSensor(&groundCheckController);
	Engine::GetInstance().box2DSensors.get()->AddSensor(&enemyCheckController);

	
}


bool Player::Update(float dt)
{
	bool previousGroundedValue = isGrounded;
	isGrounded = groundCheckController.IsBeingTriggered();

	if (isGrounded)
		isDoingFallAttack = false;
	else
		if(previousGroundedValue)
			coyoteTimer.Start();
	if (TryShovelAttack())
		isDoingShovelAttack = false;

	if (isDoingFallAttack && !isDoingShovelAttack)
	{
		enemyCheck->SetFilterData(enemyCheckFilters);
	}
	else {
		enemyCheck->SetFilterData(emptyFilter);
	}



	b2Vec2 velocity{ GetMoveInput().x * dt/1000, playerCollider->GetLinearVelocity().y};

	if (enemyCheckController.OnTriggerEnter() && isDoingFallAttack && jumpRecoverTimer.ReadMSec() >= jumpRecoverMS)
	{
		velocity.y = 0;
		playerCollider->SetLinearVelocity(velocity);

		DoJump(-jumpForce*1.15f);
	}



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
			DoJump(-jumpForce);
		}
	}


	playerCollider->SetLinearVelocity(velocity);
	SetGravityValue(playerCollider->GetLinearVelocity().y);

	if (!isGrounded && playerCollider->GetLinearVelocity().y > MAX_FALL_SPEED)
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

	//Engine::GetInstance().render.get()->DrawTexture(texture, METERS_TO_PIXELS(position.getX()+ textureOffset.x), METERS_TO_PIXELS(position.getY() + textureOffset.y),(SDL_RendererFlip)isFlipped);

	if(isDoingShovelAttack)
		animator.SelectAnimation("Player_Attack", true);
	else if (isGrounded) {
		if (velocity.x == 0)
		{
			animator.SelectAnimation("Player_Idle", true);
		}
		else
			animator.SelectAnimation("Player_Move", true);
	}
	else {
		if (isDoingFallAttack) {
			animator.SelectAnimation("Player_Fall_Attack", true);
		}
		else {
			if (playerCollider->GetLinearVelocity().y > 0)
				animator.SelectAnimation("Player_Jump_Fall", true);
			else
				animator.SelectAnimation("Player_Jump_Rise", true);
		}
	}


	animator.Update(dt);
	animator.Animate(METERS_TO_PIXELS(position.getX() + textureOffset.x), METERS_TO_PIXELS(position.getY() + textureOffset.y), (SDL_RendererFlip)isFlipped);

	Engine::GetInstance().box2DCreator.get()->RenderBody(playerCollider, b2Color{ 255,0,0,255 });
	Engine::GetInstance().box2DCreator.get()->RenderFixture(groundCheck, b2Color{0,0,255,255});
	if(isDoingFallAttack)
		Engine::GetInstance().box2DCreator.get()->RenderFixture(enemyCheck, b2Color{0,255,0,255});


	return true;
}

b2Vec2 Player::GetMoveInput() {
	b2Vec2 velocity{ 0,0 };
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		velocity.x -= (speed);

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		velocity.x += (speed);
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
	if ((isGrounded || coyoteTimer.ReadMSec() <= coyoteReactionMS) && !isDoingShovelAttack && jumpRecoverTimer.ReadMSec() >= jumpRecoverMS)
		return true;
	return false;
}

void Player::DoJump(float force) {
	
	playerCollider->ApplyForceToCenter(b2Vec2{ 0,force }, true);
	jumpRecoverTimer.Start();
}

void Player::SetGravityValue(float verticalVelocity) {
	float gravityValue = defaultGravity;

	
	if (verticalVelocity > 0) {
		gravityValue = fallGravity;
		if (isDoingFallAttack)
			gravityValue = fallAttackGravity;
	}
	playerCollider->SetGravityScale(gravityValue);
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().scene->world->DestroyBody(playerCollider);
	Engine::GetInstance().textures.get()->UnLoad(texture);
	return true;
}


