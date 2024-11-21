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
