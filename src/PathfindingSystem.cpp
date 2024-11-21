#include "PathfindingSystem.h"
#include "Engine.h"
#include "LevelManager.h"

int PathfindingSystem::GetSectionTileValue(Vector2D tile)
{
	MapLayer* layer = Engine::GetInstance().levelManager->GetCurrentSection()->GetLayerByIndex(0);
	return layer->tiles.at(tile.getY() * layer->width + tile.getX());
}

int PathfindingSystem::CalculateTilePriority(Vector2D tile)
{
	Vector2D factorCalculator = pathfinderObjects.at(currentPathfinder)->getPositionTile() - tile;
	int g = abs(factorCalculator.getX() + factorCalculator.getY());
	factorCalculator = pathfinderObjects.at(currentPathfinder)->getTargetTile() - tile;
	int h = abs(factorCalculator.getX() + factorCalculator.getY());
	return g+h;
}

PathfindingSystem::PathfindingSystem()
{
	currentPathfinder = 0;
}

Vector2D Pathfinder::toTile(Vector2D position)
{
	return { position.getX() / 16, position.getY() / 16 };
}

bool Pathfinder::NeedsToUpdate()
{
	return needsToUpdate;
}

bool PathfindingSystem::Update(float dt)
{
	for (size_t i = 0; i < pathfinderObjects.size(); i++)
	{
		if (pathfinderObjects.at(i) == nullptr)
			continue;
		pathfinderObjects.at(i)->Update(dt);
		if (!pathfinderObjects.at(i)->NeedsToUpdate())
			continue;
		//Recalculate path
	}
	return true;
}

bool PathfindingSystem::CleanUp()
{
	pathfinderObjects.clear();
	path.clear();
	alreadyCheckedTiles.clear();
	return true;
}
