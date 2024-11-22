#include "Module.h"
#include "LevelSection.h"

enum AXIS_RESTRICTION
{
	AXIS_FREE,
	AXIS_BORDER_RESTRAINT,
	AXIS_BLOCK,
};

class Pathfinder
{
	friend class PathfindingSystem;
private:
	AXIS_RESTRICTION axis_restriction[2] = {AXIS_FREE, AXIS_FREE};
	Entity* entityAttached = nullptr;
	Entity* entityTarget = nullptr;
	bool needsToUpdate = false;
	std::vector<Vector2D> path;
	Vector2D target;
	bool pause;
	bool isMoving;
	void ShortenPath();												//Done
	bool IsTileInPath(Vector2D tile);								//Done
	void RefreshTargetPosition();									//Done
	void SetNearestTileOfTarget();
protected:
	bool NeedsToUpdate();											//Done
	void Update(float dt);											//Done
public:
	Pathfinder();													//Done
	~Pathfinder();
	Vector2D toTile(Vector2D position);
	Vector2D getTargetTile();
	Vector2D getPositionTile();
	void SetTarget(Entity* entity);
	void SetTarget(Vector2D tile);
	void Connect(Entity* entity);
	void Disconnect();

	bool HasArrivedTarget();
	bool IsMoving();
	bool IsPaused();
};

class PathfindingSystem : public Module
{
	friend class Pathfinder;
private:
	int currentPathfinder;
	std::vector<Pathfinder*> pathfinderObjects;
	std::vector<Vector2D> path;
	std::vector<Vector2D> alreadyCheckedTiles;
	int GetSectionTileValue(Vector2D tile);
	int CalculateTilePriority(Vector2D tile);
protected:
	Vector2D GetTileOfEntity(Entity* entity);
	std::vector<Vector2D> GetRestrictedGroupTiles();
public:
	PathfindingSystem();
	~PathfindingSystem();

	bool Start()override;
	bool Update(float dt)override;
	bool CleanUp()override;
};