#pragma once
#include <unordered_Map>
#include "Vector2D.h"
class LevelSection;

class Level
{
public:
	Level(int levelNumber, int initialSection);
	~Level();

	bool Start();

	bool Update(float dt);

	bool CleanUp();


	int GetLevelNumber();
	LevelSection GetCurrentSection();
	int GetCurrentSectionNumber();

	Vector2D position;

private:
	int levelNumber;
	int currentSectionNumber;

	std::unordered_map<int, LevelSection> sections;
};

