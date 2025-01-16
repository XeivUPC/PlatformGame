#include "Dozedrake.h"
#include "Log.h"
#include "Engine.h"
#include "Textures.h"
#include "Box2DFactory.h"
#include "EntityManager.h"
#include "Physics.h"

Dozedrake::Dozedrake(Vector2D pos, LevelSection* layer, int id) : Enemy(pos, layer, id)
{
	enemyHealth.ModifyBaseHealth(10);
	enemyHealth.ResetHealth();
	hurtCooldown.SetInitTime(1000);
	blockedTiles = { 801, 803 };
	shootSpacing.Start();
	for (size_t i = 0; i < 4; i++)
	{
		Bubble* b = new Bubble();
		Engine::GetInstance().entityManager->AddEntity((Entity*)b);
		bubbles.emplace_back(b);
	}
}

Dozedrake::~Dozedrake()
{
}

void Dozedrake::LoadParameters()
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
		pugi::xml_node dozedrakeProperties = mapFileXML.child("entities").child("dozedrake");
		speed = dozedrakeProperties.child("speed").attribute("value").as_float();
		hitDamage = dozedrakeProperties.child("attack-damage").attribute("value").as_float();
		attackCooldownMS = dozedrakeProperties.child("attack-cooldown-ms").attribute("value").as_float();
		hurtCooldownMS = dozedrakeProperties.child("hurt-cooldown-ms").attribute("value").as_float();
		enemyHealth = Health(dozedrakeProperties.child("health").attribute("value").as_int());
		pathUpdateTime = dozedrakeProperties.child("path-finding-update-time-ms").attribute("value").as_float();
		pathDetectDistance = dozedrakeProperties.child("path-detect-distance").attribute("value").as_int();
		nearDistance = dozedrakeProperties.child("near-distance").attribute("value").as_float();
		farDistance = dozedrakeProperties.child("far-distance").attribute("value").as_float();
		textureName = dozedrakeProperties.child("texture").attribute("path").as_string();
		textureOffset = { dozedrakeProperties.child("texture").attribute("x_offset").as_float(),dozedrakeProperties.child("texture").attribute("y_offset").as_float() };
		texture = Engine::GetInstance().textures->GetTexture(textureName.c_str());

		pugi::xml_node animProperties = dozedrakeProperties.child("animator");
		animator = new Animator();
		for (pugi::xml_node animNode = animProperties.child("anim"); animNode != NULL; animNode = animNode.next_sibling("anim")) {

			std::string animName = animNode.attribute("name").as_string();
			AnimationData animData = AnimationData(animName);
			Vector2D animSize = { animNode.attribute("size-x").as_float() ,animNode.attribute("size-y").as_float() };
			for (pugi::xml_node spriteNode = animNode.child("sprite"); spriteNode != NULL; spriteNode = spriteNode.next_sibling("sprite")) {
				Vector2D spriteOffset = { spriteNode.attribute("offset-x").as_float() ,spriteNode.attribute("offset-y").as_float() };
				animData.AddSprite(Sprite{ texture, spriteOffset, animSize });
			}
			animator->AddAnimation(animData);
		}
		std::string startAnim = animProperties.attribute("start-with").as_string();
		animator->SelectAnimation(startAnim.c_str(), animProperties.attribute("loop").as_bool());
		animator->SetSpeed(animProperties.attribute("default-speed").as_float());
	}
}

void Dozedrake::InitColliders()
{
	Enemy::InitColliders();
	Box2DFactory& colliderCreator = Box2DFactory::GetInstance();
	b2World* world = Engine::GetInstance().physics->world;

	b2Vec2 enemyColliderPosition{ (position.getX()), (position.getY()) };
	enemyCollider = colliderCreator.CreateBox(world, enemyColliderPosition, PIXEL_TO_METERS(130), PIXEL_TO_METERS(75));
	enemyCollider->SetFixedRotation(true);

	b2Filter enemyFilter;
	enemyFilter.maskBits &= ~Engine::GetInstance().PLAYER_LAYER;
	enemyFilter.maskBits &= ~Engine::GetInstance().ENEMY_ATTACK_LAYER;
	enemyCollider->GetFixtureList()[0].SetFilterData(enemyFilter);

	b2FixtureUserData playerHeadCheckData;
	playerHeadCheckData.pointer = (uintptr_t)(&playerHeadCheckController);
	playerHeadCheck = colliderCreator.AddBox(enemyCollider, b2Vec2(-1.7f, 1.7f), PIXEL_TO_METERS(40), PIXEL_TO_METERS(15), playerHeadCheckData);
	playerHeadCheck->SetSensor(true);
	playerHeadCheck->SetDensity(0);
	playerHeadCheck->SetFilterData(playerFilter);

	b2FixtureUserData playerHeadDamageCheckData;
	playerHeadDamageCheckData.pointer = (uintptr_t)(&playerHeadDamageCheckController);
	playerHeadDamageCheck = colliderCreator.AddBox(enemyCollider, b2Vec2(-1.7f, 1.7f), PIXEL_TO_METERS(50), PIXEL_TO_METERS(25), playerHeadDamageCheckData);
	playerHeadDamageCheck->SetSensor(true);
	playerHeadDamageCheck->SetDensity(0);
	playerHeadDamageCheck->SetFilterData(playerDamageFilter);

	b2FixtureUserData playerBodyCheckData;
	playerBodyCheckData.pointer = (uintptr_t)(&playerCheckController);
	playerCheck = colliderCreator.AddCircle(enemyCollider, b2Vec2(1.7f, 0.5f), PIXEL_TO_METERS(35)/*, PIXEL_TO_METERS(70)*/, playerBodyCheckData);
	playerCheck->SetSensor(true);
	playerCheck->SetDensity(0);
	playerCheck->SetFilterData(playerFilter);

	b2FixtureUserData playerBodyDamageCheckData;
	playerBodyDamageCheckData.pointer = (uintptr_t)(&playerDamageCheckController);
	playerDamageCheck = colliderCreator.AddCircle(enemyCollider, b2Vec2(1.7f, 0.5f), PIXEL_TO_METERS(45)/*, PIXEL_TO_METERS(70)*/, playerBodyDamageCheckData);
	playerDamageCheck->SetSensor(true);
	playerDamageCheck->SetDensity(0);
	playerDamageCheck->SetFilterData(playerDamageFilter);

	playerCheckController.SetBodyToTrack(playerCheck);
	playerDamageCheckController.SetBodyToTrack(playerDamageCheck);
	playerHeadCheckController.SetBodyToTrack(playerHeadCheck);
	playerHeadDamageCheckController.SetBodyToTrack(playerHeadDamageCheck);
	directionBottomRightCheckController.SetBodyToTrack(directionBottomRightCheck);
	directionBottomLeftCheckController.SetBodyToTrack(directionBottomLeftCheck);

	enemyCollider->ResetMassData();
	b2MassData massData;
	massData.mass = enemyMass;
	massData.center = enemyCollider->GetLocalCenter();
	enemyCollider->SetMassData(&massData);
	basePosition = GetPosition();
	basePosition = { basePosition.getX() - levelSection->sectionOffset.x , basePosition.getY() - levelSection->sectionOffset.y };
}

void Dozedrake::Brain()
{
	Enemy::Brain();
	
	if (playerHeadDamageCheckController.IsBeingTriggered() && hurtCooldown.ReadMSec() >= hurtCooldownMS)
	{
		hurtCooldown.Start();
		Hurt();
	}
	else if (playerHeadCheckController.IsBeingTriggered() && attackCooldown.ReadMSec() >= attackCooldownMS)
	{
		attackCooldown.Start();
		Attack();
	}
	if (hurtCooldown.ReadMSec() < hurtCooldownMS/2)
	{
		isBeingHurt = true;
		isAsleep = false;
	}
	else
		isBeingHurt = false;
	enemyDirection = { 0,0 };
	if (!isAsleep && !isBeingHurt)
	{
		if (abs(player->position.getX() - position.getX()) > farDistance)
		{
			enemyDirection = { -1,0 };
		}
		else if (abs(player->position.getX() - position.getX()) < nearDistance)
		{
			enemyDirection = { 1,0 };
		}
		else
		{
			if (shootSpacing.ReadMSec() >= shootSpacingMS)
			{
				Bubble* bubble = GetBubbleToShoot();
				if (bubble != nullptr)
				{
					isAttacking = true;
					shootSpacing.Start();
					Shoot(bubble);
				}
				else
					isAttacking = false;
			}
		}
	}
	Enemy::Move();
}

bool Dozedrake::Render()
{

	Engine::GetInstance().render->SelectLayer(Render::RenderLayers::Enemy);
	animator->Animate(METERS_TO_PIXELS(position.getX()) + textureOffset.getX(), METERS_TO_PIXELS(position.getY()) + textureOffset.getY(), SDL_FLIP_HORIZONTAL);
	if (abs(enemyDirection.getX()) + abs(enemyDirection.getY()) == 0)
	{
		if (isAsleep)
			animator->SelectAnimation("Dozedrake_Sleep", true);
		else if (isAttacking)
			animator->SelectAnimation("Dozedrake_Attack", true);
		else if (isBeingHurt)
			animator->SelectAnimation("Dozedrake_Hurt", false);
		else
			animator->SelectAnimation("Dozedrake_Idle", false);
	}
	else if(enemyDirection.getX() == 1)
		animator->SelectAnimation("Dozedrake_Backward", true);
	else
		animator->SelectAnimation("Dozedrake_Forward", true);

	return true;
}

bool Dozedrake::CleanUp()
{
	bubbles.clear();
	Enemy::CleanUp();
	return true;
}

void Dozedrake::Shoot(Bubble* b)
{
	mouthPosition = Vector2D(enemyCollider->GetPosition().x-4, enemyCollider->GetPosition().y+1.5f);
	b->Throw(mouthPosition, player);
}

Bubble* Dozedrake::GetBubbleToShoot()
{
	for (size_t i = 0; i < bubbles.size(); i++)
	{
		if (!bubbles[i]->InUse())
			return bubbles[i];
	}
	return nullptr;
}
