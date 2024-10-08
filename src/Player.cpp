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
	position = Vector2D(8, 8);

	InitColliders();
	groundCheckController.SetSensor(groundCheck);
	enemyCheckController.SetSensor(enemyCheck);
	ladderCheckController.SetSensor(ladderCheck);

	attackRecoverTimer = Timer();
	jumpRecoverTimer = Timer();

	/// Texture, index, size, pivot
	jumpSoundId = Engine::GetInstance().audio->LoadFx("Player_Jump.wav");
	

	return true;
}

bool Player::Start() {

	texture = Engine::GetInstance().textures->Load(textureName.c_str());

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

	AnimationData climb = AnimationData("Player_Climb");
	climb.AddSprite(Sprite{ texture,{1.0f, 6.0f}, {70, 70} });
	climb.AddSprite(Sprite{ texture,{1.0f, 6.0f}, {70, 70} });
	climb.AddSprite(Sprite{ texture,{2.0f, 6.0f}, {70, 70} });
	climb.AddSprite(Sprite{ texture,{2.0f, 6.0f}, {70, 70} });

	AnimationData climb_exit = AnimationData("Player_Exit_Climb");
	climb_exit.AddSprite(Sprite{ texture,{0.0f, 6.0f}, {70, 70} });


	animator.AddAnimation(idle);
	animator.AddAnimation(idle2);
	animator.AddAnimation(move);
	animator.AddAnimation(jump_rise);
	animator.AddAnimation(jump_fall);
	animator.AddAnimation(fall_attack);
	animator.AddAnimation(attack);
	animator.AddAnimation(climb);
	animator.AddAnimation(climb_exit);
	animator.SelectAnimation("Player_Idle", true);

	animator.SetSpeed(100);
}

void Player::InitColliders() {

	const std::shared_ptr<Box2DCreator>& colliderCreator = Engine::GetInstance().box2DCreator;
	b2World* world = Engine::GetInstance().scene->world;

	///PlayerCollider

	b2Vec2 playerColliderPosition{ position.getX(), position.getY() };

	playerFilters.categoryBits = Engine::GetInstance().PLAYER_LAYER;
	playerFilters.maskBits = Engine::GetInstance().GROUND_LAYER;

	enemyCheckFilters.categoryBits = Engine::GetInstance().PLAYER_ATTACK_LAYER;
	enemyCheckFilters.maskBits = Engine::GetInstance().ENEMY_LAYER;

	playerLadderFilters.categoryBits = Engine::GetInstance().USE_LADDER_LAYER;
	playerLadderFilters.maskBits = Engine::GetInstance().LADDER_LAYER;

	emptyFilter.maskBits = 0x0000;
	emptyFilter.categoryBits = 0x0000;

	playerCollider = colliderCreator->CreateBox(world, playerColliderPosition, 0.8f, 1.8f);
	playerCollider->SetFixedRotation(true);
	for (b2Fixture* fixture = playerCollider->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
	{
		fixture->SetFriction(0);
		fixture->SetFilterData(playerFilters);
	}

	groundCheck = colliderCreator->AddBox(playerCollider, b2Vec2(0.0f, 0.9f), 0.7f, 0.1f);
	groundCheck->SetSensor(true);
	groundCheck->SetDensity(0);
	groundCheck->SetFilterData(playerFilters);


	enemyCheck = colliderCreator->AddBox(playerCollider, b2Vec2(0.0f, 1.0f), 0.4f, 0.1f);
	enemyCheck->SetFilterData(enemyCheckFilters);
	enemyCheck->SetFriction(0);
	enemyCheck->SetDensity(0);
	enemyCheckController.AcceptOnlyTriggers(false);

	ladderCheck = colliderCreator->AddBox(playerCollider, b2Vec2(0.0f, 0.45f), 0.4f, 0.9f);
	ladderCheck->SetFilterData(playerLadderFilters);
	ladderCheck->SetFriction(0);
	ladderCheck->SetDensity(0);
	ladderCheck->SetSensor(true);

	playerCollider->ResetMassData();

	//No necesario
	Engine::GetInstance().box2DSensors->AddSensor(&groundCheckController);
	Engine::GetInstance().box2DSensors->AddSensor(&enemyCheckController);
	Engine::GetInstance().box2DSensors->AddSensor(&ladderCheckController);
}


bool Player::Update(float dt)
{

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		Engine::GetInstance().render->camera.x -= PIXEL_TO_METERS(speed / 3) * dt;

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		Engine::GetInstance().render->camera.x += PIXEL_TO_METERS(speed / 3) * dt;

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		Engine::GetInstance().render->camera.y += PIXEL_TO_METERS(speed / 3) * dt;

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		Engine::GetInstance().render->camera.y -= PIXEL_TO_METERS(speed/3) * dt;

	bool previousGroundedValue = isGrounded;
	isGrounded = groundCheckController.IsBeingTriggered();

	if (isGrounded)
		isDoingFallAttack = false;
	else {
		if(previousGroundedValue)
			coyoteTimer.Start();
	}

	if(isInLadder)
		isDoingFallAttack = false;

	if (TryShovelAttack())
		isDoingShovelAttack = false;

	if (isDoingFallAttack && !isDoingShovelAttack)
	{
		enemyCheck->SetFilterData(enemyCheckFilters);
	}
	else {
		enemyCheck->SetFilterData(emptyFilter);
	}

	b2Vec2 inputValue = GetMoveInput();

	b2Vec2 velocity{ inputValue.x * dt, playerCollider->GetLinearVelocity().y };
	if (isInLadder) {
		velocity.y = inputValue.y * dt;
		velocity.y = PIXEL_TO_METERS(velocity.y);
	}

	velocity.x = PIXEL_TO_METERS(velocity.x);


	if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
		if (TryShovelAttack()) {
			DoShovelAttack();
		}
	}
	if (isDoingShovelAttack && isGrounded)
		velocity.x = 0;


	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		if (TryFallAttack()) {
			DoFallAttack();
		}
	}

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		if (TryJump()) {
			Engine::GetInstance().audio->PlayFx(jumpSoundId);
			velocity.y = 0;			
			playerCollider->SetLinearVelocity(velocity);
			DoJump(-jumpForce);
		}
	}

	if (ladderCheckController.IsBeingTriggered() && Engine::GetInstance().input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) {
		isInLadder = !isInLadder;
		if (!isInLadder)
			playerCollider->SetAwake(true);
	}
	else if(!ladderCheckController.IsBeingTriggered()){
		isInLadder = false;
	}

	if (enemyCheckController.OnTriggerEnter() && isDoingFallAttack && jumpRecoverTimer.ReadMSec() >= jumpRecoverMS)
	{
		velocity.y = 0;
		playerCollider->SetLinearVelocity(velocity);

		DoJump(-jumpForce * fallAttackJumpMultiplier);
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

	animator.SetIfPlaying(true);
	if (isInLadder) {
		animator.SetIfPlaying(velocity.y != 0);
		animator.SelectAnimation("Player_Climb", true);
	}
	else if(isDoingShovelAttack)
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

	Engine::GetInstance().box2DCreator->RenderBody(playerCollider, b2Color{ 255,0,0,255 });
	Engine::GetInstance().box2DCreator->RenderFixture(groundCheck, b2Color{0,0,255,255});
	Engine::GetInstance().box2DCreator->RenderFixture(ladderCheck, b2Color{255,0,255,255});
	if(isDoingFallAttack)
		Engine::GetInstance().box2DCreator->RenderFixture(enemyCheck, b2Color{0,255,0,255});
	return true;
}

b2Vec2 Player::GetMoveInput() {
	b2Vec2 velocity{ 0,0 };
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		velocity.x -= (speed);

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		velocity.x += (speed);

	if (isInLadder) {
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			velocity.y-= (ladderSpeed);

		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			velocity.y += (ladderSpeed);
	}

	return velocity;

}
bool Player::TryShovelAttack() {
	if (attackRecoverTimer.ReadMSec() >= attackRecoverMS && !isInLadder)
		return true;
	return false;
}
void Player::DoShovelAttack() {
	attackRecoverTimer.Start();
	isDoingShovelAttack = true;
}

bool Player::TryFallAttack() {
	if (!isGrounded && !isDoingFallAttack && !isInLadder)
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

	if (isInLadder) {
		gravityValue = 0;
	}
	playerCollider->SetGravityScale(gravityValue);
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().scene->world->DestroyBody(playerCollider);
	Engine::GetInstance().textures->UnLoad(texture);
	return true;
}


