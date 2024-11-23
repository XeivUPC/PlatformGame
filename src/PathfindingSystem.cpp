#include "PathfindingSystem.h"
#include "LevelManager.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Scene.h"
#include "Player.h"
#include "Log.h"

Pathfinding::Pathfinding(std::vector<int> tiles, int widht, int height) {

    tileX = Engine::GetInstance().textures->Load("Assets/Textures/Path.png");
    //Loads texture to draw the path
    layerNav = new MapLayer();
    layerNav->tiles = tiles;
    layerNav->width = widht;
    layerNav->height = height;

    // Initialize the costSoFar with all elements set to 0
    costSoFar = std::vector<std::vector<int>>(layerNav->width, std::vector<int>(layerNav->height, 0));
}

Pathfinding::~Pathfinding() {
    delete layerNav;
}

// L11: BFS Pathfinding methods
void Pathfinding::ResetPath(Vector2D pos) {

    // Clear the frontier queue
    while (!frontier.empty()) {
        frontier.pop();
    }

    // Clear the frontierDijkstra queue
    while (!frontierDijkstra.empty()) {
        frontierDijkstra.pop();
    }

    // Clear the frontierAStar queue
    while (!frontierAStar.empty()) {
        frontierAStar.pop();
    }

    visited.clear(); //Clear the visited list
    breadcrumbs.clear(); //Clear the breadcrumbs list
    pathTiles.clear(); //Clear the pathTiles list

    // Inserts the first position in the queue and visited list
    frontier.push(pos); //BFS
    frontierDijkstra.push(std::make_pair(0, pos)); //Dijkstra
    frontierAStar.push(std::make_pair(0, pos)); //AStar
    visited.push_back(pos);
    breadcrumbs.push_back(pos);

    //reset the costSoFar matrix
    costSoFar = std::vector<std::vector<int>>(layerNav->width, std::vector<int>(layerNav->height, 0));

}

void Pathfinding::DrawPath() {

    // Draw path
    Engine::GetInstance().render->LockLayer(Render::RenderLayers::Layer7);
    for (const auto& pathTile : visited) {
        SDL_Rect rect = { 32,0,16,16 };
        Engine::GetInstance().render.get()->DrawTexture(tileX, METERS_TO_PIXELS(pathTile.getX()), METERS_TO_PIXELS(pathTile.getY()+1), SDL_FLIP_NONE, &rect);
    }



    std::priority_queue<std::pair<int, Vector2D>, std::vector<std::pair<int, Vector2D>>, std::greater<std::pair<int, Vector2D>> > frontierAStarCopy = frontierAStar;

    // Iterate over the elements of the frontier copy
    while (!frontierAStarCopy.empty()) {

        //Get the first element of the queue
        Vector2D frontierTile = frontierAStarCopy.top().second;
        //Get the position of the frontier tile in the world
        SDL_Rect rect = { 16,0,16,16 };
        Engine::GetInstance().render.get()->DrawTexture(tileX, METERS_TO_PIXELS(frontierTile.getX()), METERS_TO_PIXELS(frontierTile.getY()+1), SDL_FLIP_NONE, &rect);
        //Remove the front element from the queue
        frontierAStarCopy.pop();
    }
    
    for (size_t i = 0; i < pathTiles.size(); i++)
    {
        SDL_Rect tileRect = { 0,0,16,16 };
        Vector2D pathTile = pathTiles[i];
        Engine::GetInstance().render->DrawTexture(tileX,METERS_TO_PIXELS(pathTile.getX()), METERS_TO_PIXELS(pathTile.getY()+1), SDL_FLIP_NONE);
    }

    Engine::GetInstance().render->UnlockLayer();
}

bool Pathfinding::IsWalkable(int x, int y) {

    bool isWalkable = false;

    // L11: TODO 3: return true only if x and y are within map limits
    // and the tile is walkable (tile id 0 in the navigation layer)

    //Set isWalkable to true if the position is inside map limits and is a position that is not blocked in the navigation layer
    //Get the navigation layer

    if (layerNav != nullptr) {
        if (x >= 0 && y >= 0 && x < layerNav->width && y < layerNav->height) {
            int gid = layerNav->Get(x, y);
            if (gid != blockedGid) isWalkable = true;
        }
    }

    return isWalkable;
}


void Pathfinding::PropagateAStar(ASTAR_HEURISTICS heuristic) {

    // L13: TODO 2: Adapt Dijkstra algorithm for AStar. Consider the different heuristics

    Vector2D playerPos = Engine::GetInstance().scene.get()->player->position;
    playerPos = { (float)METERS_TO_PIXELS((int)playerPos.getX()),(float)METERS_TO_PIXELS((int)playerPos.getY()) };
    Vector2D playerPosTile = MapToWorld(playerPos);

    bool foundDestination = false;
    if (frontierAStar.size() > 0) {
        Vector2D frontierTile = frontierAStar.top().second;

        if (frontierTile == playerPosTile) {
            foundDestination = true;

            //When the destination is reach, call the function ComputePath
            ComputePath(frontierTile.getX(), frontierTile.getY());
        }
    }

    //If frontier queue contains elements pop the first element and find the neighbors
    if (frontierAStar.size() > 0 && !foundDestination) {

        //Get the value of the firt element in the queue
        Vector2D frontierTile = frontierAStar.top().second;
        //remove the first element from the queue
        frontierAStar.pop();

        std::list<Vector2D> neighbors;
        if (IsWalkable(frontierTile.getX() + 1, frontierTile.getY())) {
            neighbors.push_back(Vector2D((int)frontierTile.getX() + 1, (int)frontierTile.getY()));
        }
        if (IsWalkable(frontierTile.getX(), frontierTile.getY() + 1)) {
            neighbors.push_back(Vector2D((int)frontierTile.getX(), (int)frontierTile.getY() + 1));
        }
        if (IsWalkable(frontierTile.getX() - 1, frontierTile.getY())) {
            neighbors.push_back(Vector2D((int)frontierTile.getX() - 1, (int)frontierTile.getY()));
        }
        if (IsWalkable(frontierTile.getX(), frontierTile.getY() - 1)) {
            neighbors.push_back(Vector2D((int)frontierTile.getX(), (int)frontierTile.getY() - 1));
        }

        //For each neighbor, if not visited, add it to the frontier queue and visited list
        for (const auto& neighbor : neighbors) {

            // the movement cost from the start point A to the current tile.
            int g = costSoFar[(int)frontierTile.getX()][(int)frontierTile.getY()] + MovementCost((int)neighbor.getX(), (int)neighbor.getY());

            // the estimated movement cost from the current square to the destination point.
            int h = 0;

            switch (heuristic)
            {
            case ASTAR_HEURISTICS::MANHATTAN:
                h = neighbor.distanceMahattan(playerPosTile);
                break;
            case ASTAR_HEURISTICS::EUCLIDEAN:
                h = neighbor.distanceEuclidean(playerPosTile);
                break;
            case ASTAR_HEURISTICS::SQUARED:
                h = neighbor.distanceSquared(playerPosTile);
                break;
            }

            // A* Priority function
            int f = g + h;

            if (std::find(visited.begin(), visited.end(), neighbor) == visited.end() || g < costSoFar[neighbor.getX()][neighbor.getY()]) {
                costSoFar[neighbor.getX()][neighbor.getY()] = g;
                frontierAStar.push(std::make_pair(f, neighbor));
                visited.push_back(neighbor);
                breadcrumbs.push_back(frontierTile);
            }
        }

    }
}

int Pathfinding::MovementCost(int x, int y)
{
    int ret = -1;

    if ((x >= 0) && (x < layerNav->width) && (y >= 0) && (y < layerNav->height))
    {
        int gid = layerNav->Get(x, y);
        if (gid == highCostGid) {
            ret = 5;
        }
        else ret = 1;
    }

    return ret;
}

void Pathfinding::ComputePath(int x, int y)
{
    // L12: TODO 2: Follow the breadcrumps to goal back to the origin
    // at each step, add the point into "pathTiles" (it will then draw automatically)

    //Clear the pathTiles list
    pathTiles.clear();
    // Save tile position received and stored in current tile
    Vector2D currentTile = Vector2D(x, y);
    // Add the current tile to the pathTiles list (is the first element in the path)
    pathTiles.push_back(currentTile);
    // Find the position of the current tile in the visited list
    int index = Find(visited, currentTile);

    // while currentTile and breadcrumbs[index] are different it means we are not in the origin
    while ((index >= 0) && (currentTile != breadcrumbs[index]))
    {
        // update the current tile to the previous tile in the path
        currentTile = breadcrumbs[index];
        // Add the current tile to the pathTiles list
        pathTiles.push_back(currentTile);
        // Find the position of the current tile in the visited list
        index = Find(visited, currentTile);
    }
}

int Pathfinding::Find(std::vector<Vector2D> vector, Vector2D elem)
{
    int index = 0;
    bool found = false;
    for (const auto& e : vector) {
        if (e == elem) {
            found = true;
            break;
        }
        index++;
    }

    if (found) return index;
    else return -1;

}

Vector2D Pathfinding::MapToWorld(Vector2D pos)
{
    b2Vec2 offsetSection = Engine::GetInstance().levelManager->GetCurrentSection()->sectionOffset;
    Vector2D ret = { (pos.getX() - METERS_TO_PIXELS(offsetSection.x)) , pos.getY() - METERS_TO_PIXELS(offsetSection.y)};

    ret.setX(PIXEL_TO_METERS(ret.getX()));
    ret.setY(PIXEL_TO_METERS(ret.getY()));

    return ret;

}
