#pragma once
#include <list>
#include <box2d/box2d.h>
#include <SDL2/SDL.h>
#include "pugixml.hpp"
#include "Vector2D.h"
#include <string>

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
    std::string name;
    int width;
    int height;
    unsigned int* tiles;

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
    std::list<MapLayer*> layers;
};

using namespace pugi;

class LevelSection
{
public:
	LevelSection();
	~LevelSection();
	bool Update(float dt);
	bool CleanUp();
    bool Load(std::string fileName, std::string texturePath);

private:

    Vector2D MapToWorld(int x, int y) const;

    TileSet* CreateTileset(xml_node* node, std::string texturePath);
    MapLayer* CreateMapLayer(xml_node* node);
    void CreateMapData(xml_document* document);
    b2Body* CreateColliders(xml_node* node);

    void AddLayers(uint16* container, std::string layersInput);
    MapData mapData;

    std::list<b2Body*> colliders;

	
};



