#pragma once
#include <list>
#include <box2d/box2d.h>
#include <SDL2/SDL.h>
#include "pugixml.hpp"
#include "Vector2D.h"
#include "Entity.h"
#include "AnimationSystem.h"
#include <string>
#include <unordered_map>

// Create a struct needed to hold the information to Map node
struct TileSet
{
    int firstgid;
    std::string name;
    int tilewidth;
    int tileheight;
    int tilecount;
    int columns;

    SDL_Texture* texture;

    // Implement the method that receives the gid and returns a Rect

    SDL_Rect GetRect(unsigned int gid) {
        SDL_Rect rect = { 0 };

        int relativeIndex = gid - firstgid;
        rect.w = tilewidth;
        rect.h = tileheight;
        rect.x = (tilewidth) * (relativeIndex % columns);
        rect.y = (tileheight) * (relativeIndex / columns);

        return rect;
    }
};

struct MapLayer
{
    // Add the info to the MapLayer Struct
    int id;
    int layerIndex;
    std::string name;
    int width;
    int height;
    std::vector<int> tiles;

    // Short function to get the gid value of x,y
    unsigned int Get(int x, int y) const
    {
        return tiles[(y * width) + x];
    }
};

struct MapData
{
    int width;
    int height;
    int tilewidth;
    int tileheight;
    
    std::list<TileSet*> tilesets;

    //Add a list/array of layers to the map
    std::vector<MapLayer*> layers;
};

using namespace pugi;

class LevelSection
{
public:
	LevelSection();
	~LevelSection();
	bool Update(float dt);
	bool CleanUp();
    bool Load(std::string fileName, std::string texturePath, b2Vec2 offset  = b2Vec2_zero, bool loadColliders = true , bool loadObjects = true, bool loadEnemies=true);
    void LoadColliders();
    void LoadObjects();
    void LoadEnemies();

    MapLayer* GetLayerByIndex(int index);

    int sectionNumber;

    int leftSection;
    int rightSection;
    int topSection;
    int bottomSection;

    MapData mapData;

    b2Vec2 sectionOffset;
    std::vector<Entity*> objects;

private:
    Vector2D MapToWorld(int x, int y) const;

    TileSet* CreateTileset(xml_node* node, std::string texturePath);
    MapLayer* CreateMapLayer(xml_node* node, int layerIndex);
    void CreateMapData(xml_document* document);
    b2Body* CreateColliders(xml_node* node);

    void AddLayers(uint16* container, std::string layersInput);

    std::list<b2Body*> colliders;

    pugi::xml_document mapFileXML;
    pugi::xml_node mapNode;

    std::unordered_map<int, Animator*> animatedTiles;
};



