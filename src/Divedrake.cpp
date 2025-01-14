#include "Divedrake.h"
#include "Engine.h"
#include "Physics.h"
#include "Textures.h"
#include "Box2DFactory.h"
#include "Box2DRender.h"
#include "Log.h"

Divedrake::Divedrake(Vector2D pos, LevelSection* layer, int id) : Enemy(pos, layer, id)
{
	enemyHealth.ModifyBaseHealth(1);
	enemyHealth.ResetHealth();

	blockedTiles = { 803, 804 };
}

Divedrake::~Divedrake()
{

}

void Divedrake::LoadParameters()
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
		pugi::xml_node divedrakeProperties = mapFileXML.child("entities").child("divedrake"); //// Cambiar al diverdrake
		speed = divedrakeProperties.child("speed").attribute("value").as_float();
		hitDamage = divedrakeProperties.child("attack-damage").attribute("value").as_float();
		attackCooldownMS = divedrakeProperties.child("attack-cooldown-ms").attribute("value").as_float();
		hurtCooldownMS = divedrakeProperties.child("hurt-cooldown-ms").attribute("value").as_float();
		enemyHealth = Health(divedrakeProperties.child("health").attribute("value").as_int());
		pathUpdateTime = divedrakeProperties.child("path-finding-update-time-ms").attribute("value").as_float();
		pathDetectDistance = divedrakeProperties.child("path-detect-distance").attribute("value").as_int();


		textureName = divedrakeProperties.child("texture").attribute("path").as_string();
		textureOffset = { divedrakeProperties.child("texture").attribute("x_offset").as_float(),divedrakeProperties.child("texture").attribute("y_offset").as_float() };
		texture = Engine::GetInstance().textures->GetTexture(textureName.c_str());

		pugi::xml_node animProperties = divedrakeProperties.child("animator");
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

void Divedrake::InitColliders()
{
	Enemy::InitColliders();
	Box2DFactory& colliderCreator = Box2DFactory::GetInstance();
	b2World* world = Engine::GetInstance().physics->world;

	b2Vec2 enemyColliderPosition{ (position.getX()), (position.getY()) };
	enemyCollider = colliderCreator.CreateBox(world, enemyColliderPosition, PIXEL_TO_METERS(2), PIXEL_TO_METERS(2));
	enemyCollider->SetFixedRotation(true);
	enemyCollider->SetGravityScale(0);

	b2Filter enemyFilter;
	enemyFilter.maskBits &= ~Engine::GetInstance().PLAYER_LAYER;
	enemyFilter.maskBits &= ~Engine::GetInstance().ENEMY_ATTACK_LAYER;
	enemyCollider->GetFixtureList()[0].SetFilterData(enemyFilter);


	b2FixtureUserData playerCheckData;
	playerCheckData.pointer = (uintptr_t)(&playerCheckController);
	playerCheck = colliderCreator.AddBox(enemyCollider, b2Vec2(0.0f, 0.0f), PIXEL_TO_METERS(28), PIXEL_TO_METERS(12), playerCheckData);
	playerCheck->SetSensor(true);
	playerCheck->SetDensity(0);
	playerCheck->SetFilterData(playerFilter);

	b2FixtureUserData playerDamageCheckData;
	playerDamageCheckData.pointer = (uintptr_t)(&playerDamageCheckController);
	playerDamageCheck = colliderCreator.AddBox(enemyCollider, b2Vec2(0.0f, 0.0f), PIXEL_TO_METERS(36), PIXEL_TO_METERS(20), playerDamageCheckData);
	playerDamageCheck->SetSensor(true);
	playerDamageCheck->SetDensity(0);
	playerDamageCheck->SetFilterData(playerDamageFilter);

	playerCheckController.SetBodyToTrack(playerCheck);

	playerDamageCheckController.SetBodyToTrack(playerDamageCheck);
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

void Divedrake::Brain()
{
	if (pathUpdateTime < pathUpdateTimer.ReadMSec())
	{
		pathUpdateTimer.Start();
		Vector2D offset = { levelSection->sectionOffset.x, levelSection->sectionOffset.y };
		Vector2D startPos = { position.getX() - offset.getX(), position.getY() - offset.getY() };
		Vector2D targetPos = { player->position.getX() - offset.getX(), player->position.getY() - offset.getY() };
		Engine::GetInstance().pathfinding->FindPath(levelSection->mapData.layers.at(4)->tiles, levelSection->mapData.layers.at(4)->width, levelSection->mapData.layers.at(4)->height, blockedTiles, startPos, targetPos, pathDetectDistance);
		while (!Engine::GetInstance().pathfinding->HasFinished())
		{
			Engine::GetInstance().pathfinding->PropagateAStar(SQUARED);
		}
		if (Engine::GetInstance().pathfinding->HasFound())
		{
		}

		pathData = Engine::GetInstance().pathfinding->GetData();
	}
	FindCurrentTileInPath();
	SetPathDirection();
	if (enemyDirection.getX() == 0 && enemyDirection.getY() == 0)
	{
		Vector2D offset = { levelSection->sectionOffset.x, levelSection->sectionOffset.y };
		Vector2D startPos = { position.getX() - offset.getX(), position.getY() - offset.getY() };
		Engine::GetInstance().pathfinding->FindPath(levelSection->mapData.layers.at(4)->tiles, levelSection->mapData.layers.at(4)->width, levelSection->mapData.layers.at(4)->height, blockedTiles, startPos, basePosition, pathDetectDistance, true);
		while (!Engine::GetInstance().pathfinding->HasFinished())
		{
			Engine::GetInstance().pathfinding->PropagateAStar(SQUARED);
		}

		pathData = Engine::GetInstance().pathfinding->GetData();
		FindCurrentTileInPath();
		SetPathDirection();
	}
	Enemy::Brain();
	Enemy::Move();
}

void Divedrake::Render(float dt)
{
	Engine::GetInstance().pathfinding->DrawPath(&pathData, { levelSection->sectionOffset.x, levelSection->sectionOffset.y });
	Enemy::Render(dt);
	if(abs(enemyDirection.getX()) <= 0.2f)
		animator->SelectAnimation("Divedrake_Idle", true);
	else
	{
			if(enemyDirection.getY() < -0.2f)
				animator->SelectAnimation("Divedrake_Up", true);

			else if (enemyDirection.getY() > 0.2f)
				animator->SelectAnimation("Divedrake_Down", true);

			else
				animator->SelectAnimation("Divedrake_Side", true);
	}
}