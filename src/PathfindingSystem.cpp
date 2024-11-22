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

PathfindingSystem::~PathfindingSystem()
{
}

Pathfinder::Pathfinder()
{
	target = getPositionTile();
}

Pathfinder::~Pathfinder()
{
}

Vector2D Pathfinder::toTile(Vector2D position)
{
	return { position.getX() / 16, position.getY() / 16 };
}

Vector2D Pathfinder::getTargetTile()
{
	return target;
}

Vector2D Pathfinder::getPositionTile()
{
	return toTile(entityAttached->position);
}

void Pathfinder::SetTarget(Entity* entity)
{
	entityTarget = entity;
	RefreshTargetPosition();
}

void Pathfinder::SetTarget(Vector2D tile)
{
	target = tile;
	entityTarget = nullptr;
}

void Pathfinder::Connect(Entity* entity)
{
	entityAttached = entity;
	SetTarget(entity);
	path.clear();
}

void Pathfinder::Disconnect()
{
	entityAttached = nullptr;
	entityTarget = nullptr;
}

bool Pathfinder::HasArrivedTarget()
{
	if (entityAttached->position.getX() == target.getX() && entityAttached->position.getY() == target.getY())
		return true;
	return false;
}

void Pathfinder::ShortenPath()
{
	int size = 0;
	for (int i = 0; i < path.size(); i++)
	{
		if (path.at(i).getX() == getPositionTile().getX() && path.at(i).getY() == getPositionTile().getY())
		{
			size = i;
			break;
		}
	}
	for (int i = 0; i < path.size() - size; i++)
	{
		path.at(i) = path.at(i + size);
	}
	size = path.size() - size;
	for (int i = size-1; i >= 0; i--)
	{
		if (path.at(i).getX() == getTargetTile().getX() && path.at(i).getY() == getTargetTile().getY())
		{
			size -= i;
			path.resize(size);
			break;
		}
	}
}

bool Pathfinder::IsTileInPath(Vector2D tile)
{
	for (size_t i = 0; i < path.size(); i++)
	{
		if (path.at(i).getX() == tile.getX() && path.at(i).getY() == tile.getY())
			return true;
	}
	return false;
}

void Pathfinder::RefreshTargetPosition()
{
	if (entityTarget == nullptr)return;
	target = toTile(entityTarget->position);
}

bool Pathfinder::NeedsToUpdate()
{
	return needsToUpdate;
}

void Pathfinder::Update(float dt)
{
	RefreshTargetPosition();
	if (IsTileInPath(target) && IsTileInPath(entityAttached->position))
		ShortenPath();
	else
		needsToUpdate = true;
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
