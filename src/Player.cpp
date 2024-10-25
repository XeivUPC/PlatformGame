#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Box2DFactory.h"
#include "Box2DRender.h"
#include "CollisionsManager.h"
#include "LevelManager.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Physics.h"
#include "Log.h"
#include "Debug.h"
#include "UI.h"



Player::Player() : Entity(EntityType::PLAYER)
{
	name = "Player";
}

Player::~Player() {
	
}

bool Player::Awake() {

	//Initialize Player parameters
	SetPosition({ 8,8 });

	InitColliders();
	groundCheckController.SetBodyToTrack(groundCheck);
	shovelFallAttackCheckController.SetBodyToTrack(shovelFallAttackCheck);
	ladderCheckController.SetBodyToTrack(ladderCheck);

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

	animator = new Animator();
	AnimationData idle = AnimationData("Player_Idle");
	idle.AddSprite(Sprite{ texture,{0.0f, 0.0f}, {70, 70}});

	AnimationData idle2 = AnimationData("Player_Idle_2");
	idle2.AddSprite(Sprite{ texture,{2.0f, 0.0f}, {70, 70}});
	idle2.AddSprite(Sprite{ texture,{3.0f, 0.0f}, {70, 70}});
	idle2.AddSprite(Sprite{ texture,{4.0f, 0.0f}, {70, 70}});

	AnimationData crouch = AnimationData("Player_Crouch");
	crouch.AddSprite(Sprite{ texture,{6.0f, 0.0f}, {70, 70} });


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


	
	animator->AddAnimation(idle);
	animator->AddAnimation(idle2);
	animator->AddAnimation(crouch);
	animator->AddAnimation(move);
	animator->AddAnimation(jump_rise);
	animator->AddAnimation(jump_fall);
	animator->AddAnimation(fall_attack);
	animator->AddAnimation(attack);
	animator->AddAnimation(climb);
	animator->AddAnimation(climb_exit);
	animator->SelectAnimation("Player_Idle", true);

	animator->SetSpeed(100);
}

void Player::InitColliders() {

	Box2DFactory& colliderCreator = Box2DFactory::GetInstance();
	b2World* world = Engine::GetInstance().physics->world;

	///PlayerCollider
	
	b2Vec2 playerColliderPosition{ (position.getX()), (position.getY()) };

	playerFilters.categoryBits = Engine::GetInstance().PLAYER_LAYER;
	playerFilters.maskBits = Engine::GetInstance().GROUND_LAYER | Engine::GetInstance().INTERACTABLE_LAYER | Engine::GetInstance().ENEMY_LAYER;

	groundCheckFilters.categoryBits = Engine::GetInstance().PLAYER_LAYER;
	groundCheckFilters.maskBits = Engine::GetInstance().GROUND_LAYER;

	enemyCheckFilters.categoryBits = Engine::GetInstance().PLAYER_ATTACK_LAYER;
	enemyCheckFilters.maskBits = Engine::GetInstance().ENEMY_LAYER;

	playerLadderFilters.categoryBits = Engine::GetInstance().USE_LADDER_LAYER;
	playerLadderFilters.maskBits = Engine::GetInstance().LADDER_LAYER;

	emptyFilter.maskBits = 0x0000;
	emptyFilter.categoryBits = 0x0000;

	playerCollider = colliderCreator.CreateBevelBox(world, playerColliderPosition, PIXEL_TO_METERS(15), PIXEL_TO_METERS(30), PIXEL_TO_METERS(1));
	playerCollider->SetFixedRotation(true);
	for (b2Fixture* fixture = playerCollider->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
	{
		fixture->SetFriction(0);
		fixture->SetFilterData(playerFilters);
	}

	b2FixtureUserData groundCheckData;
	groundCheckData.pointer = (uintptr_t)(&groundCheckController);
	groundCheck = colliderCreator.AddBox(playerCollider, b2Vec2(0.0f, PIXEL_TO_METERS(10.5f)), PIXEL_TO_METERS(14), PIXEL_TO_METERS(10), groundCheckData);
	groundCheck->SetSensor(true);
	groundCheck->SetDensity(0);

	groundCheck->SetFilterData(groundCheckFilters);


	b2FixtureUserData shovelFallAttackData;
	shovelFallAttackData.pointer = (uintptr_t)(&shovelFallAttackCheckController);
	shovelFallAttackCheck = colliderCreator.AddBox(playerCollider, b2Vec2(0.0f, PIXEL_TO_METERS(16.5f)), PIXEL_TO_METERS(12), PIXEL_TO_METERS(2), shovelFallAttackData);
	shovelFallAttackCheck->SetFilterData(enemyCheckFilters);
	shovelFallAttackCheck->SetFriction(0);
	shovelFallAttackCheck->SetDensity(0);
	shovelFallAttackCheckController.AcceptOnlyTriggers(false);

	shovelAttackCheckRight = colliderCreator.AddBox(playerCollider, b2Vec2(PIXEL_TO_METERS(20), PIXEL_TO_METERS(3)), PIXEL_TO_METERS(25), PIXEL_TO_METERS(10));
	shovelAttackCheckRight->SetFilterData(enemyCheckFilters);
	shovelAttackCheckRight->SetFriction(0);
	shovelAttackCheckRight->SetDensity(0);

	shovelAttackCheckLeft = colliderCreator.AddBox(playerCollider, b2Vec2(PIXEL_TO_METERS(-20), PIXEL_TO_METERS(3)), PIXEL_TO_METERS(25), PIXEL_TO_METERS(10));
	shovelAttackCheckLeft->SetFilterData(enemyCheckFilters);
	shovelAttackCheckLeft->SetFriction(0);
	shovelAttackCheckLeft->SetDensity(0);


	b2FixtureUserData ladderCheckData;
	ladderCheckData.pointer = (uintptr_t)(&ladderCheckController);
	ladderCheck = colliderCreator.AddBox(playerCollider, b2Vec2(0.0f, PIXEL_TO_METERS(7)), PIXEL_TO_METERS(8), PIXEL_TO_METERS(15), ladderCheckData);
	ladderCheck->SetFilterData(playerLadderFilters);
	ladderCheck->SetFriction(0);
	ladderCheck->SetDensity(0);
	ladderCheck->SetSensor(true);

	playerCollider->ResetMassData();

	b2MassData massData;
	massData.mass = playerMass;
	massData.center = playerCollider->GetLocalCenter();
	playerCollider->SetMassData(&massData);
}


bool Player::Update(float dt)
{
	float fixedDt = Engine::GetInstance().GetMaxFrameDuration()/1000.0f;
	playerCollider->SetAwake(true);

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		Damage(10, {isFlipped ? -1.0f : 1.0f, 1.0f });
	}

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT)
		coins.Add(1);

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)
		magic.Add(1);

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		if(!Engine::GetInstance().ui->pauseMenuUI.isActive)
			Engine::GetInstance().ui->pauseMenuUI.Activate();
		else
			Engine::GetInstance().ui->pauseMenuUI.Deactivate();
	}


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

	if (isDoingFallAttack && !isDoingShovelAttack && playerCollider->GetLinearVelocity().y>0)
	{
		shovelFallAttackCheck->SetFilterData(enemyCheckFilters);
	}
	else {
		shovelFallAttackCheck->SetFilterData(emptyFilter);
	}

	if (Engine::GetInstance().debug->HasDebug(2))
		isInvulnerable = true;

	if (isInvulnerable && !Engine::GetInstance().debug->HasDebug(2)) {
		if (hurtAnimTimeMS <= hurtAnimTimeTimer.ReadMSec()) {
			isInvulnerable = false;
		}
	}




	b2Vec2 inputValue = GetMoveInput();

	b2Vec2 velocity{ inputValue.x * fixedDt, playerCollider->GetLinearVelocity().y };
	if (isInLadder)
		velocity.y = inputValue.y * fixedDt;


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
		else {
			isDoingFallAttack = false;
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

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		isCrouching = true;
	else
		isCrouching = false;

	if (ladderCheckController.IsBeingTriggered() && Engine::GetInstance().input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) {
		isInLadder = !isInLadder;
	}
	else if(!ladderCheckController.IsBeingTriggered()){
		isInLadder = false;
	}

	if (shovelFallAttackCheckController.OnTriggerEnter() && isDoingFallAttack && jumpRecoverTimer.ReadMSec() >= jumpRecoverMS)
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



	if (isDoingShovelAttack && attackRecoverTimer.ReadMSec() <= attackRecoverMS / 2 && !isFlipped)
	{
		shovelAttackCheckRight->SetFilterData(enemyCheckFilters);
	}
	else {
		shovelAttackCheckRight->SetFilterData(emptyFilter);
	}


	if (isDoingShovelAttack && attackRecoverTimer.ReadMSec() <= attackRecoverMS / 2 && isFlipped)
	{
		shovelAttackCheckLeft->SetFilterData(enemyCheckFilters);
	}
	else {
		shovelAttackCheckLeft->SetFilterData(emptyFilter);
	}


	
	animator->SetIfPlaying(true);

	if (isInvulnerable) {
		if (hurtAnimEffectTimer.ReadMSec() >= hurtAnimEffectMS) {
			animator->SetIfCanDraw(!animator->CanDraw());
			hurtAnimEffectTimer.Start();
		}
	}
	else
		animator->SetIfCanDraw(true);

	if (isInLadder) {
		animator->SetIfPlaying(velocity.y != 0);
		animator->SelectAnimation("Player_Climb", true);
	}
	else if(isDoingShovelAttack)
		animator->SelectAnimation("Player_Attack", true);
	else if (isGrounded) {
		if (velocity.x == 0)
		{
			if (isCrouching)
				animator->SelectAnimation("Player_Crouch", true);
			else
				animator->SelectAnimation("Player_Idle", true);
		}
		else
			animator->SelectAnimation("Player_Move", true);
	}
	else {
		if (isDoingFallAttack) {
			animator->SelectAnimation("Player_Fall_Attack", true);
		}
		else {
			if (playerCollider->GetLinearVelocity().y > 0)
				animator->SelectAnimation("Player_Jump_Fall", true);
			else
				animator->SelectAnimation("Player_Jump_Rise", true);
		}
	}

	Engine::GetInstance().render->LockLayer(Render::RenderLayers::Layer3);
	animator->Update(dt);
	animator->Animate(METERS_TO_PIXELS(position.getX()) + textureOffset.x, METERS_TO_PIXELS(position.getY()) + textureOffset.y, (SDL_RendererFlip)isFlipped);

	if (Engine::GetInstance().debug->HasDebug(1))
	{
		Engine::GetInstance().render->LockLayer(Render::RenderLayers::Layer7);

		Box2DRender::GetInstance().RenderBody(playerCollider, b2Color{ 255,0,0,255 });
		Box2DRender::GetInstance().RenderFixture(groundCheck, b2Color{ 0,0,255,255 });
		Box2DRender::GetInstance().RenderFixture(ladderCheck, b2Color{ 255,0,255,255 });

		if (isDoingShovelAttack && attackRecoverTimer.ReadMSec() <= attackRecoverMS / 2 && !isFlipped) {
			Box2DRender::GetInstance().RenderFixture(shovelAttackCheckRight, b2Color{ 255,255,255,255 });
		}
		if (isDoingShovelAttack && attackRecoverTimer.ReadMSec() <= attackRecoverMS / 2 && isFlipped) {
			Box2DRender::GetInstance().RenderFixture(shovelAttackCheckLeft, b2Color{ 255,255,255,255 });
		}
		if (isDoingFallAttack && playerCollider->GetLinearVelocity().y > 0 && !isDoingShovelAttack) {
			Box2DRender::GetInstance().RenderFixture(shovelFallAttackCheck, b2Color{ 0,255,0,255 });
		}
	}
	Engine::GetInstance().render->UnlockLayer();


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
	delete animator;
	Engine::GetInstance().physics->world->DestroyBody(playerCollider);
	Engine::GetInstance().textures->UnLoad(texture);
	return true;
}

void Player::Damage(int amount, Vector2D direction)
{
	if (isInvulnerable)
		return;

	playerHealth.Hurt(amount);
	hurtAnimEffectTimer.Start();
	hurtAnimTimeTimer.Start();

	playerCollider->ApplyForceToCenter({50.0f * direction.getX(),-120 * direction.getY()}, true);

	isInvulnerable = true;

	if (!playerHealth.IsAlive()) {
		Vector2D spawnPos = Engine::GetInstance().levelManager->GetClosestCheckPointPosition();
		playerCollider->SetTransform({ (spawnPos.getX()),(spawnPos.getY() - 1) }, 0);
		position.setX(playerCollider->GetPosition().x);
		position.setY(playerCollider->GetPosition().y);
		Engine::GetInstance().levelManager->GoToClosestCheckPoint();
		playerCollider->SetAwake(true);
		playerHealth.ResetHealth();
		isInvulnerable = false;
	}
}

void Player::SetPosition(Vector2D pos)
{
	if(playerCollider!=nullptr)
		playerCollider->SetTransform({ pos.getX(), pos.getY() }, 0);
	position = pos;
}


