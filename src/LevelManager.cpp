#include "LevelManager.h"
#include "Log.h"
#include "Engine.h"
#include "Render.h"
#include "Scene.h"
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
	LoadLevel(parameters.child("firstLevel").attribute("level").as_int());
	ChargeAllLevelSection(1);
	LoadSection(parameters.child("startingSection").attribute("section").as_int());

	return true;
}

bool LevelManager::Update(float dt)
{

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
		GoToNextSection(b2Vec2{ 1,0 });
	else if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		GoToNextSection(b2Vec2{ -1,0 });
	else if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
		GoToNextSection(b2Vec2{ 0,1 });
	else if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
		GoToNextSection(b2Vec2{ 0,-1 });

	for (size_t i = 0; i < sectionsInUse.size(); i++)
	{
		loadedSections[sectionsInUse[i]]->Update(dt);
	}

	//for (const auto& pair : loadedSections) {
	//	if (pair.second != nullptr) {
	//		pair.second->Update(dt);
	//	}
	//}
	return true;
}

bool LevelManager::CleanUp()
{
	for (const auto& pair : loadedSections) {
		if (pair.second != nullptr) {
			pair.second->CleanUp();
			delete pair.second;
		}	
	}
	loadedSections.clear();

	return true;
}

bool LevelManager::LoadLevel(int levelToPlay)
{
	CleanUp();
	currentLevel = levelToPlay;

	levelsPath = path + "Level" + std::to_string(currentLevel) +"/";
	texturePath = path;

	Engine::GetInstance().audio->PlayMusic(("Level"+ std::to_string(currentLevel) +"_Music.ogg").c_str());

	return true;
}

bool LevelManager::ChargeAdjacentSections(LevelSection* mainSection)
{
	if (mainSection == nullptr)
		return false;

	if (mainSection->leftSection != -1 && loadedSections[mainSection->leftSection] == nullptr) {
		LevelSection* sectionLeft = new LevelSection();
		sectionLeft->Load(levelsPath + "Level" + std::to_string(currentLevel) + " - Sector" + std::to_string(mainSection->leftSection) + ".tmx", texturePath, b2Vec2(-mainSection->mapData.width * mainSection->mapData.tilewidth + mainSection->sectionOffset.x, mainSection->sectionOffset.y), false);

		sectionLeft->sectionOffset.x = -sectionLeft->mapData.width * sectionLeft->mapData.tilewidth + mainSection->sectionOffset.x;
		sectionLeft->LoadColliders();
		loadedSections[mainSection->leftSection] = sectionLeft;
	}
	if (mainSection->rightSection != -1 && loadedSections[mainSection->rightSection] == nullptr) {
		LevelSection* sectionRight = new LevelSection();
		sectionRight->Load(levelsPath + "Level" + std::to_string(currentLevel) + " - Sector" + std::to_string(mainSection->rightSection) + ".tmx", texturePath, b2Vec2(mainSection->mapData.width * mainSection->mapData.tilewidth + mainSection->sectionOffset.x, mainSection->sectionOffset.y));
		loadedSections[mainSection->rightSection] = sectionRight;
	}

	if (mainSection->topSection != -1 && loadedSections[mainSection->topSection] == nullptr) {
		LevelSection* sectionTop = new LevelSection();
		sectionTop->Load(levelsPath + "Level" + std::to_string(currentLevel) + " - Sector" + std::to_string(mainSection->topSection) + ".tmx", texturePath, b2Vec2(mainSection->sectionOffset.x, -mainSection->mapData.height * mainSection->mapData.tileheight + mainSection->sectionOffset.y));
		loadedSections[mainSection->topSection] = sectionTop;
	}

	if (mainSection->bottomSection != -1 && loadedSections[mainSection->bottomSection] == nullptr) {
		LevelSection* sectionBottom = new LevelSection();
		sectionBottom->Load(levelsPath + "Level" + std::to_string(currentLevel) + " - Sector" + std::to_string(mainSection->bottomSection) + ".tmx", texturePath, b2Vec2(mainSection->sectionOffset.x, mainSection->mapData.height * mainSection->mapData.tileheight + mainSection->sectionOffset.y));
		loadedSections[mainSection->bottomSection] = sectionBottom;
	}
	return true;
}

bool LevelManager::ChargeAllLevelSection(int startingIndex)
{
	while (true) {
		
		if (startingIndex == 1) {
			LevelSection* section = new LevelSection();
			section->Load(levelsPath + "Level" + std::to_string(currentLevel) + " - Sector" + std::to_string(startingIndex) + ".tmx", texturePath, {0,16*2});
			loadedSections[startingIndex] = section;
		}
		else {
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

	Vector2D minPos = { PIXEL_TO_METERS(sectionToLoad->sectionOffset.x), PIXEL_TO_METERS(sectionToLoad->sectionOffset.y ) };
	Vector2D maxPos = { PIXEL_TO_METERS(sectionToLoad->sectionOffset.x) + sectionToLoad->mapData.width,PIXEL_TO_METERS(sectionToLoad->sectionOffset.y) + sectionToLoad->mapData.height };
	Engine::GetInstance().GetInstance().render->SetConfinementValues(minPos,maxPos);

	return true;
}

void LevelManager::GoToNextSection(b2Vec2 direction)
{
	if (direction == b2Vec2_zero)
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

LevelSection* LevelManager::GetCurrentSection()
{
	return loadedSections[currentSection];
}
