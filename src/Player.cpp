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



Player::Player() : Entity(EntityType::PLAYER)
{
	name = "Player";
}

Player::~Player() {
	
}

bool Player::Awake() {

	//Initialize Player parameters
	SetPosition({ 8,8 });
	return true;
}

bool Player::Start() 
{
	LoadParameters();

	InitColliders();

	groundCheckController.SetBodyToTrack(groundCheck);
	shovelFallAttackCheckController.SetBodyToTrack(shovelFallAttackCheck);
	ladderCheckController.SetBodyToTrack(ladderCheck);

	attackRecoverTimer = Timer();
	jumpRecoverTimer = Timer();

	isInvulnerable = true;

	return true;
}


void Player::InitColliders() {

	Box2DFactory& colliderCreator = Box2DFactory::GetInstance();
	b2World* world = Engine::GetInstance().physics->world;

	///PlayerCollider
	
	b2Vec2 playerColliderPosition{ (position.getX()), (position.getY()) };

	playerFilters.categoryBits = Engine::GetInstance().PLAYER_LAYER;
	playerFilters.maskBits = Engine::GetInstance().GROUND_LAYER | Engine::GetInstance().INTERACTABLE_LAYER | Engine::GetInstance().ENEMY_ATTACK_LAYER;

	groundCheckFilters.categoryBits = Engine::GetInstance().PLAYER_LAYER;
	groundCheckFilters.maskBits = Engine::GetInstance().GROUND_LAYER;

	enemyCheckFilters.categoryBits = Engine::GetInstance().PLAYER_ATTACK_LAYER;
	enemyCheckFilters.maskBits = Engine::GetInstance().ENEMY_LAYER;

	playerLadderFilters.categoryBits = Engine::GetInstance().USE_LADDER_LAYER;
	playerLadderFilters.maskBits = Engine::GetInstance().LADDER_LAYER;

	emptyFilter.maskBits = 0x0000;
	emptyFilter.categoryBits = 0x0000;

	playerCollider = colliderCreator.CreateBevelBox(world, playerColliderPosition, PIXEL_TO_METERS(14), PIXEL_TO_METERS(30), PIXEL_TO_METERS(1));
	playerCollider->SetFixedRotation(true);
	for (b2Fixture* fixture = playerCollider->GetFixtureList(); fixture != nullptr; fixture = fixture->GetNext())
	{
		fixture->SetFriction(0);
		fixture->SetFilterData(playerFilters);
	}

	b2FixtureUserData groundCheckData;
	groundCheckData.pointer = (uintptr_t)(&groundCheckController);
	groundCheck = colliderCreator.AddBox(playerCollider, b2Vec2(0.0f, PIXEL_TO_METERS(10.5f)), PIXEL_TO_METERS(13), PIXEL_TO_METERS(10), groundCheckData);
	groundCheck->SetSensor(true);
	groundCheck->SetDensity(0);

	groundCheck->SetFilterData(groundCheckFilters);


	b2FixtureUserData shovelFallAttackData;
	shovelFallAttackData.pointer = (uintptr_t)(&shovelFallAttackCheckController);
	shovelFallAttackCheck = colliderCreator.AddBox(playerCollider, b2Vec2(0.0f, PIXEL_TO_METERS(16.5f)), PIXEL_TO_METERS(12), PIXEL_TO_METERS(8), shovelFallAttackData);
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
	float fixedDt = 16/1000.0f;
	playerCollider->SetAwake(true);

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		Damage(10, {isFlipped ? -1.0f : 1.0f, 1.0f });
	}

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT)
		coins.Add(1);

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT)
		magic.Add(1);

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		//if(!Engine::GetInstance().ui->pauseMenuUI.isActive)
			//Engine::GetInstance().ui->pauseMenuUI.Activate();
		//else
			//Engine::GetInstance().ui->pauseMenuUI.Deactivate();
	}


	bool previousGroundedValue = isGrounded;
	isGrounded = groundCheckController.IsBeingTriggered();

	if(previousGroundedValue){
		isDoingFallAttack = false;
		coyoteTimer.Start();
	}

	if(isInLadder)
		isDoingFallAttack = false;

	if (TryShovelAttack())
		isDoingShovelAttack = false;

	if (isDoingFallAttack && !isDoingShovelAttack && playerCollider->GetLinearVelocity().y>=0)
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


	if (isDead && deathTimeTimer.ReadMSec() >= deathTimeMS) {
		Respawn();
	}

	b2Vec2 inputValue = GetMoveInput();
	if (isDead) {
		inputValue = { 0,0 };
	}

	b2Vec2 velocity{ inputValue.x * fixedDt, playerCollider->GetLinearVelocity().y };
	if (isInLadder && !isDead)
		velocity.y = inputValue.y * fixedDt;

	if (Engine::GetInstance().debug->HasDebug(2)) {
		velocity.x = inputValue.x * fixedDt;
		velocity.y = inputValue.y * fixedDt;
	}

	


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
			velocity.y = playerCollider->GetLinearVelocity().y;
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
		velocity.y = playerCollider->GetLinearVelocity().y;
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

	animator->SetSpeed(100);
	if (isDead) {
		animator->SetSpeed(1200);
		if (isGrounded) {
			animator->SelectAnimation("Player_Dead_Ground", false);
		}
		else {
			animator->SelectAnimation("Player_Dead_Air", true);
		}
	}
	else if (isInLadder) {
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

	if (Engine::GetInstance().debug->HasDebug(2)) {
		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			velocity.y -= (speed);

		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			velocity.y += (speed);
	}

	return velocity;

}
bool Player::TryShovelAttack() {
	if (attackRecoverTimer.ReadMSec() >= attackRecoverMS && !isInLadder && !isDead)
		return true;
	return false;
}
void Player::DoShovelAttack() {
	attackRecoverTimer.Start();
	isDoingShovelAttack = true;
}

bool Player::TryFallAttack() {
	if (!isGrounded && !isDoingFallAttack && !isInLadder && !isDead)
		return true;
	return false;
}
void Player::DoFallAttack() {
	isDoingFallAttack = true;
}


bool Player::TryJump() {
	if ((isGrounded || coyoteTimer.ReadMSec() <= coyoteReactionMS) && !isDoingShovelAttack && jumpRecoverTimer.ReadMSec() >= jumpRecoverMS && !isDead)
		return true;
	return false;
}

void Player::DoJump(float force) {
	playerCollider->ApplyLinearImpulseToCenter(b2Vec2{ 0,force }, true);
	
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

	if (Engine::GetInstance().debug->HasDebug(2)) {
		gravityValue = 0;
	}


	playerCollider->SetGravityScale(gravityValue);
}

void Player::LoadParameters()
{
	bool ret = true;
	pugi::xml_document mapFileXML;
	pugi::xml_parse_result result = mapFileXML.load_file("entityData.xml");
	if (result == NULL)
	{
		LOG("Could not load map xml file entityData.xml. pugi error: %s", result.description());
		ret = false;
	}
	else {
		pugi::xml_node playerProperties = mapFileXML.child("entities").child("player");
		speed = playerProperties.child("speed").attribute("value").as_float();
		ladderSpeed = playerProperties.child("ladder-speed").attribute("value").as_float();
		MAX_FALL_SPEED = playerProperties.child("max-fall-speed").attribute("value").as_float();
		jumpForce = playerProperties.child("jump-force").attribute("value").as_float();
		fallAttackJumpMultiplier = playerProperties.child("fall-attack-jump-multiplier").attribute("value").as_float();
		playerHealth = Health(playerProperties.child("player-health").attribute("value").as_int());
		defaultGravity = playerProperties.child("default-gravity").attribute("value").as_float();
		fallGravity = playerProperties.child("fall-gravity").attribute("value").as_float();
		fallAttackGravity = playerProperties.child("fall-attack-gravity").attribute("value").as_float();
		attackDamage = playerProperties.child("attack-damage").attribute("value").as_float();
		coyoteReactionMS = playerProperties.child("coyote-reaction-ms").attribute("value").as_float();
		attackRecoverMS = playerProperties.child("attack-recover-ms").attribute("value").as_float();
		exitLadderTimeMS = playerProperties.child("exit-ladder-time-ms").attribute("value").as_float();
		jumpRecoverMS = playerProperties.child("jump-recover-ms").attribute("value").as_float();
		deathTimeMS = playerProperties.child("death-time-ms").attribute("value").as_float();

		textureName = playerProperties.child("texture").attribute("path").as_string();
		textureOffset = { playerProperties.child("texture").attribute("x_offset").as_float(),playerProperties.child("texture").attribute("y_offset").as_float() };
		texture = Engine::GetInstance().textures->Load(textureName.c_str());

		std::string fileName = playerProperties.child("jump-sound-id").attribute("value").as_string();
		jumpSoundId = Engine::GetInstance().audio->LoadFx(fileName.c_str());
		fileName = playerProperties.child("hurt-sound-id").attribute("value").as_string();
		hurtSoundId = Engine::GetInstance().audio->LoadFx(fileName.c_str());
		fileName = playerProperties.child("die-sound-id").attribute("value").as_string();
		dieSoundId = Engine::GetInstance().audio->LoadFx(fileName.c_str());


		pugi::xml_node animProperties = playerProperties.child("animator");
		animator = new Animator();
		for (pugi::xml_node animNode = animProperties.child("anim"); animNode != NULL; animNode = animNode.next_sibling("anim")){

			std::string animName = animNode.attribute("name").as_string();
			AnimationData animData = AnimationData(animName);
			Vector2D animSize = { animNode.attribute("size-x").as_float() ,animNode.attribute("size-y").as_float() };
			for (pugi::xml_node spriteNode = animNode.child("sprite"); spriteNode != NULL; spriteNode = spriteNode.next_sibling("sprite")){
				Vector2D spriteOffset = { spriteNode.attribute("offset-x").as_float() ,spriteNode.attribute("offset-y").as_float() };
				animData.AddSprite(Sprite{texture, spriteOffset, animSize});
			}
			animator->AddAnimation(animData);
		}
		std::string startAnim = animProperties.attribute("start-with").as_string();
		animator->SelectAnimation(startAnim.c_str(), animProperties.attribute("loop").as_bool());
		animator->SetSpeed(animProperties.attribute("default-speed").as_float());
	}
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
	if (isInvulnerable || isDead)
		return;

	playerHealth.Hurt(amount);
	hurtAnimEffectTimer.Start();
	hurtAnimTimeTimer.Start();

	playerCollider->ApplyLinearImpulseToCenter({50.0f * direction.getX(),-2 * direction.getY()}, true);

	isInvulnerable = true;
	Engine::GetInstance().audio->PlayFx(hurtSoundId);
	if (!playerHealth.IsAlive()) {

		Engine::GetInstance().audio->PlayFx(dieSoundId);

		isInvulnerable = false;
		isDead = true;
		deathTimeTimer.Start();
	}

}

void Player::Respawn()
{
	isDead = false;
	Vector2D spawnPos = Engine::GetInstance().levelManager->GetClosestCheckPointPosition();
	playerCollider->SetTransform({ (spawnPos.getX()),(spawnPos.getY() - 1) }, 0);
	position.setX(playerCollider->GetPosition().x);
	position.setY(playerCollider->GetPosition().y);
	playerCollider->SetAwake(true);
	playerHealth.ResetHealth();
	isInvulnerable = false;
	Engine::GetInstance().levelManager->GoToClosestCheckPoint();
}

void Player::SetPosition(Vector2D pos)
{
	if(playerCollider!=nullptr)
		playerCollider->SetTransform({ pos.getX(), pos.getY() }, 0);
	position = pos;
}

Vector2D Player::GetPosition()
{
	return { playerCollider->GetPosition().x ,playerCollider->GetPosition().y};
}


