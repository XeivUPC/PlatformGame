
#pragma once
#include "Module.h"
#include "LevelSection.h"

#include <list>
#include <queue>
#include <functional> // for std::greater
#include "Vector2D.h"
#include "SDL2/SDL.h"

enum ASTAR_HEURISTICS {
    MANHATTAN = 0,
    EUCLIDEAN,
    SQUARED
};


class Pathfinding
{

public:

    Pathfinding(std::vector<int> tiles, int widht, int height);

    ~Pathfinding();

    // L11: BFS Pathfinding methods
    void ResetPath(Vector2D pos);
    void DrawPath();
    bool IsWalkable(int x, int y);

    // L12: Methods for BFS + Pathfinding and cost function for Dijkstra
    int MovementCost(int x, int y);
    void ComputePath(int x, int y);

    // L13: A* Pathfinding methods
    void PropagateAStar(ASTAR_HEURISTICS heuristic);

private:
    int Find(std::vector<Vector2D> vector, Vector2D elem);
    Vector2D MapToWorld(Vector2D pos);
public:

    // L11: BFS Pathfinding variables
    MapLayer* layerNav;
    std::queue<Vector2D> frontier;
    std::vector<Vector2D> visited;
    SDL_Texture* pathTex = nullptr;
    Vector2D destination;

    // L12: Dijkstra Pathfinding variables
    std::priority_queue<std::pair<int, Vector2D>, std::vector<std::pair<int, Vector2D>>, std::greater<std::pair<int, Vector2D>> > frontierDijkstra;
    std::vector<Vector2D> breadcrumbs; //list of tiles that form the path
    std::vector<std::vector<int>> costSoFar; //matrix that stores the accumulated cost in the propagation of the Dijkstra algorithm
    std::vector<Vector2D> pathTiles; //list of tiles that form the path
    SDL_Texture* tileX = nullptr; //texture used to show the path 

    // L13: A* Pathfinding variables
    std::priority_queue<std::pair<int, Vector2D>, std::vector<std::pair<int, Vector2D>>, std::greater<std::pair<int, Vector2D>> > frontierAStar;

    int blockedGid = 803; //Gid of the tiles that block the path - Important adjust this value to your map
    int highCostGid = 8010; //Gid of the tiles that have high cost - Important adjust this value to your map

};