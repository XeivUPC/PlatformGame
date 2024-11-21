#include "Module.h"
#include "LevelSection.h"

class Pathfinder
{
private:
	std::vector<Vector2D> path;
	Vector2D target;
	bool needsToUpdate;
public:
	Pathfinder();
	~Pathfinder();
	Vector2D toTile();
	Vector2D getTargetTile();
	Vector2D getPositionTile();
};

class PathfindingSystem : public Module
{
private:
	int currentPathfinder;
	std::vector<Pathfinder*> pathfinderObjects;
	std::vector<Vector2D> path;
	std::vector<Vector2D> alreadyCheckedTiles;
	int GetSectionTileValue(Vector2D tile);
	int CalculateTilePriority(Vector2D tile);
public:
	PathfindingSystem();
	~PathfindingSystem();

};