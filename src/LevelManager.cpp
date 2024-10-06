#include "LevelManager.h"
#include "Log.h"

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
	LoadSection(parameters.child("startingSection").attribute("section").as_int());
	return true;
}

bool LevelManager::Update(float dt)
{
	LevelSection* current = loadedSections[currentSection];
	if (current != nullptr)
	{
		current->Update(dt);
	}

	return true;
}

bool LevelManager::CleanUp()
{
	for (const auto& pair : loadedSections) {
		pair.second->CleanUp();
		delete pair.second;
	}
	loadedSections.clear();

	return true;
}

bool LevelManager::LoadLevel(int levelToPlay)
{
	currentLevel = levelToPlay;

	levelsPath = path + "Level" + std::to_string(currentLevel) +"/";
	texturePath = path;

	return true;
}

bool LevelManager::LoadSection(int sectionNumber)
{
	currentSection = sectionNumber;

	if (loadedSections[currentSection] != nullptr)
		return true;

	LevelSection* section = new LevelSection();

	section->Load(levelsPath + "Level" + std::to_string(currentLevel) + " - Sector" + std::to_string(sectionNumber) + ".tmx", texturePath);

	loadedSections[currentSection] = section;

	return true;
}
