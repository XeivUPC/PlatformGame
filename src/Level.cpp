#include "Level.h"
#include "LOG.h"
#include "LevelSection.h"

Level::Level(int levelNumber, int initialSection)
{
	/// Load Sections



	///

	this->levelNumber = levelNumber;
	currentSectionNumber = initialSection;

	if (!sections[currentSectionNumber].LoadSectionData())
	{
		LOG("Failed Loading Section --> Level %d - %d", this->levelNumber, currentSectionNumber);
	}
}

Level::~Level()
{
}

bool Level::Start()
{
	return true;
}

bool Level::Update(float dt)
{
	return true;
}

bool Level::CleanUp()
{
	return true;
}

int Level::GetLevelNumber()
{
	return levelNumber;
}

LevelSection Level::GetCurrentSection()
{
	return sections[currentSectionNumber];
}


int Level::GetCurrentSectionNumber()
{
	return currentSectionNumber;
}
