#include "Module.h"
#include "LevelSection.h"

class Pathfinder
{
	friend class PathfindingSystem;
private:
	bool needsToUpdate;
	std::vector<Vector2D> path;
	Vector2D target;
protected:
	bool NeedsToUpdate();
	void Update(float dt);
public:
	Pathfinder();
	~Pathfinder();
	Vector2D toTile(Vector2D position);
	Vector2D getTargetTile();
	Vector2D getPositionTile();
	void Connect();
	void Disconnect();
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

	bool Awake()override;
	bool Start()override;
	bool PreUpdate()override;
	bool Update(float dt)override;
	bool PostUpdate()override;
	bool CleanUp()override;

};