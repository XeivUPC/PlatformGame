#include "LevelManager.h"
#include "EntityManager.h"
#include "Log.h"
#include "Engine.h"
#include "Entity.h"
#include "Render.h"
#include "Scene.h"
#include "Textures.h"
#include "Audio.h"

LevelManager::LevelManager()
{
	name = "levels";
}

LevelManager::~LevelManager()
{

}

bool LevelManager::Awake()
{
	return true;
}

bool LevelManager::LoadParameters(xml_node parameters)
{
	LOG("Loading Map Parser");
	bool ret = true;
	this->parameters = parameters;
	path = parameters.attribute("path").as_string();

	return true;
}

bool LevelManager::Start()
{
	
	return true;
}

bool LevelManager::Update(float dt)
{
	if(currentLevel == -1)
		LoadLevel(0);
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_1) == KEY_DOWN){

		LoadLevel(1);
	}

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) {

		LoadLevel(0);
		
	}

	for (size_t i = 0; i < sectionsInUse.size(); i++)
	{
		loadedSections[sectionsInUse[i]]->Update(dt);
	}

	return true;
}

bool LevelManager::CleanUp()
{
	levelLoaded = false;
	LOG("Unloading map");
	for (const auto& pair : loadedSections) {
		if (pair.second != nullptr) {
			pair.second->CleanUp();
			delete pair.second;
		}	
	}

	for (CheckPoint* checkPoint : checkPoints) {
		if (checkPoint != nullptr) {
			Engine::GetInstance().entityManager->DestroyEntityAtUpdateEnd((Entity*)checkPoint);
		}
	}

	loadedSections.clear();
	checkPoints.clear();
	sectionsInUse.clear();
	return true;
}

bool LevelManager::LoadLevel(int levelToPlay)
{
	CleanUp();
	currentLevel = levelToPlay;

	levelsPath = path + "Level" + std::to_string(currentLevel) +"/";
	texturePath = path;

	//Engine::GetInstance().audio->PlayMusic(("Level"+ std::to_string(currentLevel) +"_Music.ogg").c_str());

	ChargeAllLevelSection(1);
	LoadSection(1);

	levelLoaded = true;

	return true;
}

bool LevelManager::ChargeAdjacentSections(LevelSection* mainSection)
{
	if (mainSection == nullptr)
		return false;

	if (mainSection->leftSection != -1 && loadedSections.count(mainSection->leftSection) == 0) {
		LevelSection* sectionLeft = new LevelSection();
		sectionLeft->Load(levelsPath + "Level" + std::to_string(currentLevel) + " - Sector" + std::to_string(mainSection->leftSection) + ".tmx", texturePath, b2Vec2(-mainSection->mapData.width + mainSection->sectionOffset.x, mainSection->sectionOffset.y), false, false, false);
		sectionLeft->sectionOffset.x = -sectionLeft->mapData.width * sectionLeft->mapData.tilewidth + mainSection->sectionOffset.x;
		sectionLeft->LoadColliders();
		sectionLeft->LoadObjects();
		sectionLeft->LoadEnemies();
		loadedSections[mainSection->leftSection] = sectionLeft;
	}
	if (mainSection->rightSection != -1 && loadedSections.count(mainSection->rightSection) == 0) {
		LevelSection* sectionRight = new LevelSection();
		sectionRight->Load(levelsPath + "Level" + std::to_string(currentLevel) + " - Sector" + std::to_string(mainSection->rightSection) + ".tmx", texturePath, b2Vec2(mainSection->mapData.width + mainSection->sectionOffset.x, mainSection->sectionOffset.y));
		loadedSections[mainSection->rightSection] = sectionRight;
	}

	if (mainSection->topSection != -1 && loadedSections.count(mainSection->topSection) == 0) {
		LevelSection* sectionTop = new LevelSection();
		sectionTop->Load(levelsPath + "Level" + std::to_string(currentLevel) + " - Sector" + std::to_string(mainSection->topSection) + ".tmx", texturePath, b2Vec2(mainSection->sectionOffset.x, -mainSection->mapData.height + mainSection->sectionOffset.y));
		loadedSections[mainSection->topSection] = sectionTop;
	}

	if (mainSection->bottomSection != -1 && loadedSections.count(mainSection->bottomSection) == 0) {
		LevelSection* sectionBottom = new LevelSection();
		sectionBottom->Load(levelsPath + "Level" + std::to_string(currentLevel) + " - Sector" + std::to_string(mainSection->bottomSection) + ".tmx", texturePath, b2Vec2(mainSection->sectionOffset.x, mainSection->mapData.height + mainSection->sectionOffset.y));
		loadedSections[mainSection->bottomSection] = sectionBottom;
	}
	return true;
}

bool LevelManager::ChargeAllLevelSection(int startingIndex)
{
	while (true) {
		
		if (startingIndex == 1) {
			LevelSection* section = new LevelSection();
			std::string sectionPath = levelsPath + "Level" + std::to_string(currentLevel) + " - Sector" + std::to_string(startingIndex) + ".tmx";
			section->Load(sectionPath, texturePath, {0,1});
			loadedSections[startingIndex] = section;
		}
		else {
			if (loadedSections.count(startingIndex - 1) == 0)
				break;
			if (!ChargeAdjacentSections(loadedSections[startingIndex-1]))
				break;
		}
		startingIndex++;
	}
	return true;
}

bool LevelManager::LoadSection(int sectionNumber)
{
	if (sectionNumber == -1)
		return true;
	if (loadedSections[sectionNumber] == nullptr)
		return true;

	currentSection = sectionNumber;
	LevelSection* sectionToLoad = loadedSections[currentSection];

	sectionsInUse.clear();

	sectionsInUse.push_back(currentSection);

	if (sectionToLoad->leftSection != -1)
		sectionsInUse.push_back(sectionToLoad->leftSection);
	if (sectionToLoad->rightSection != -1)
		sectionsInUse.push_back(sectionToLoad->rightSection);
	if (sectionToLoad->topSection != -1)
		sectionsInUse.push_back(sectionToLoad->topSection);
	if (sectionToLoad->bottomSection != -1)
		sectionsInUse.push_back(sectionToLoad->bottomSection);

	Vector2D minPos = { (sectionToLoad->sectionOffset.x), (sectionToLoad->sectionOffset.y ) };
	Vector2D maxPos = { (sectionToLoad->sectionOffset.x) + sectionToLoad->mapData.width,(sectionToLoad->sectionOffset.y) + sectionToLoad->mapData.height };
	Engine::GetInstance().GetInstance().render->SetConfinementValues(minPos,maxPos);

	return true;
}

void LevelManager::GoToNextSection(b2Vec2 direction)
{
	if (direction == b2Vec2_zero || !levelLoaded)
		return;
	if (direction.x == 1) {
		LoadSection(loadedSections[currentSection]->rightSection);

	}
	if (direction.x == -1) {
		LoadSection(loadedSections[currentSection]->leftSection);

	}
	if (direction.y == 1) {
		LoadSection(loadedSections[currentSection]->topSection);

	}
	if (direction.y == -1) {
		LoadSection(loadedSections[currentSection]->bottomSection);

	}

}

void LevelManager::GoToClosestCheckPoint()
{
	int sectionToGo = INT16_MAX;
	bool finded = false;
	for (size_t i = 0; i < checkPoints.size(); i++)
	{
		CheckPoint* checkPoint = checkPoints[i];
		if (checkPoint->IsUnlocked()) {
			int sectionPlaced = checkPoint->GetSection();
			if (currentSection >= sectionPlaced || sectionToGo>sectionPlaced) {
				sectionToGo = sectionPlaced;
				finded = true;
			}
		}
	}
	if (finded) {
		LoadSection(sectionToGo);
	}
	else
		LoadSection(1);
	Engine::GetInstance().GetInstance().render->ConfineCameraBetweenRange();
}

Vector2D LevelManager::GetClosestCheckPointPosition()
{
	int sectionToGo = INT16_MAX;
	bool finded = false;

	Vector2D pos{8,8};
	for (size_t i = 0; i < checkPoints.size(); i++)
	{
		CheckPoint* checkPoint = checkPoints[i];
		if (checkPoint->IsUnlocked()) {
			int sectionPlaced = checkPoint->GetSection();
			if (currentSection >= sectionPlaced || sectionToGo > sectionPlaced) {
				sectionToGo = sectionPlaced;
				finded = true;
				pos = checkPoint->GetRespawnPos();
			}
		}
	}
	return pos;
}

void LevelManager::RegisterCheckPoint(CheckPoint* checkPoint)
{
	checkPoints.emplace_back(checkPoint);
}

LevelSection* LevelManager::GetCurrentSection()
{
	return loadedSections[currentSection];
}
