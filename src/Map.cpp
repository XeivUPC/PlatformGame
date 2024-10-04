
#include "Engine.h"
#include "Render.h"
#include "Textures.h"
#include "Box2DCreator.h"
#include "Scene.h"
#include "Map.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
    name = "map";
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    

    return ret;
}

bool Map::Start() {

    //Calls the function to load the map. The name of the map is assigned in teh scene
    Load(mapPath + mapName);

    currentLevel = new Level(0, 0);
    currentLevel->Start();

    b2World* world = Engine::GetInstance().scene.get()->world;
    b2Vec2 position{ PIXEL_TO_METERS(200), PIXEL_TO_METERS(240) };
    b2Vec2 position2{ PIXEL_TO_METERS(100), PIXEL_TO_METERS(220) };
    b2Vec2 position3{ PIXEL_TO_METERS(200), PIXEL_TO_METERS(220) };

    b2Filter filter;
    filter.categoryBits = Engine::GetInstance().GROUND_LAYER;
    filter.maskBits = Engine::GetInstance().PLAYER_LAYER;

    groundCollider = Engine::GetInstance().box2DCreator.get()->CreateBox(world, position, PIXEL_TO_METERS(400), PIXEL_TO_METERS(20));
    enemyTest = Engine::GetInstance().box2DCreator.get()->CreateBox(world, position3, PIXEL_TO_METERS(20), PIXEL_TO_METERS(5));
    groundCollider2 = Engine::GetInstance().box2DCreator.get()->CreateBox(world, position2, PIXEL_TO_METERS(50), PIXEL_TO_METERS(20));
    groundCollider->SetType(b2_staticBody);
    groundCollider2->SetType(b2_staticBody);
    enemyTest->SetType(b2_staticBody);

    groundCollider->GetFixtureList()[0].SetFilterData(filter);
    groundCollider2->GetFixtureList()[0].SetFilterData(filter);


    filter.categoryBits = Engine::GetInstance().ENEMY_LAYER;
    filter.maskBits = Engine::GetInstance().PLAYER_ATTACK_LAYER ;

    enemyTest->GetFixtureList()[0].SetFilterData(filter);

    return true;
}

bool Map::Update(float dt)
{
    bool ret = true;

    if(mapLoaded == false)
        return false;

    currentLevel->Update(dt);

    // Prepare the loop to draw all tiles in a layer + DrawTexture()
    // iterate all tiles in a layer
    for (const auto& mapLayer : mapData.layers) {

        for (int i = 0; i < mapData.width; i++) {
            for (int j = 0; j < mapData.height; j++) {
                //Get the gid from tile
                int gid = mapLayer->Get(i, j);
                //Get the Rect from the tileSetTexture;
                SDL_Rect tileRect = mapData.tilesets.front()->GetRect(gid);
                //Get the screen coordinates from the tile coordinates
                Vector2D mapCoord = MapToWorld(i, j);

                // Complete the draw function
                Engine::GetInstance().render->DrawTexture(mapData.tilesets.front()->texture, mapCoord.getX(), mapCoord.getY(),SDL_FLIP_NONE, &tileRect);

            }
        }
    }

    Engine::GetInstance().box2DCreator.get()->RenderBody(groundCollider, b2Color{ 0,255,0,255 });
    Engine::GetInstance().box2DCreator.get()->RenderBody(groundCollider2, b2Color{ 0,255,0,255 });
    Engine::GetInstance().box2DCreator.get()->RenderBody(enemyTest, b2Color{255,0,0,255 });
    return ret;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    currentLevel->CleanUp();

    // Make sure you clean up any memory allocated from tilesets/map
    for (const auto& tileset : mapData.tilesets) {
        delete tileset;
	}
	mapData.tilesets.clear();

    // clean up all layer data
    for(const auto& layer:mapData.layers)
	{
		delete layer;
	}
    mapData.layers.clear();

    return true;
}

// Load new map
bool Map::Load(std::string mapFileName)
{
    bool ret = true;

    // L05: DONE 3: Implement LoadMap to load the map properties
    // retrieve the paremeters of the <map> node and save it into map data

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.c_str());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
        ret = false;
    }
    else {

        //Fill mapData variable
        mapData.width = mapFileXML.child("map").attribute("width").as_int();
        mapData.height = mapFileXML.child("map").attribute("height").as_int();
        mapData.tilewidth = mapFileXML.child("map").attribute("tilewidth").as_int();
        mapData.tileheight = mapFileXML.child("map").attribute("tileheight").as_int();

        // L05: DONE 4: Implement the LoadTileSet function to load the tileset properties
       // Iterate the Tileset
        for (pugi::xml_node tilesetNode = mapFileXML.child("map").child("tileset"); tilesetNode != NULL; tilesetNode = tilesetNode.next_sibling("tileset")) {

            TileSet* tileset = new TileSet();

            //Load Tileset attributes
            tileset->name = tilesetNode.attribute("name").as_string();
            tileset->firstgid = tilesetNode.attribute("firstgid").as_int();
            tileset->margin = tilesetNode.attribute("margin").as_int();
            tileset->spacing = tilesetNode.attribute("spacing").as_int();
            tileset->tilewidth = tilesetNode.attribute("tilewidth").as_int();
            tileset->tileheight = tilesetNode.attribute("tileheight").as_int();
            tileset->columns = tilesetNode.attribute("columns").as_int();

            //Load Tileset image
            std::string mapTex = mapPath;
            mapTex += tilesetNode.child("image").attribute("source").as_string();
            tileset->texture = Engine::GetInstance().textures.get()->Load(mapTex.c_str());

            mapData.tilesets.push_back(tileset);

        }

        // L06: DONE 3: Iterate all layers in the TMX and load each of them
        for (pugi::xml_node layerNode = mapFileXML.child("map").child("layer"); layerNode != NULL; layerNode = layerNode.next_sibling("layer")) {

            // L06: DONE 4: Implement a function that loads a single layer layer
            //Load the attributes and saved in a new MapLayer
            MapLayer* mapLayer = new MapLayer();
            mapLayer->id = layerNode.attribute("id").as_int();
            mapLayer->name = layerNode.attribute("name").as_string();
            mapLayer->width = layerNode.attribute("width").as_int();
            mapLayer->height = layerNode.attribute("height").as_int();

            //Reserve the memory for the data 
            mapLayer->tiles = new unsigned int[mapLayer->width * mapLayer->height];
            memset(mapLayer->tiles, 0, mapLayer->width * mapLayer->height);

            //Iterate over all the tiles and assign the values in the data array
            int i = 0;
            for (pugi::xml_node tileNode = layerNode.child("data").child("tile"); tileNode != NULL; tileNode = tileNode.next_sibling("tile")) {
                mapLayer->tiles[i] = tileNode.attribute("gid").as_uint();
                i++;
            }

            //add the layer to the map
            mapData.layers.push_back(mapLayer);
        }

        // L05: DONE 5: LOG all the data loaded iterate all tilesetsand LOG everything
        if (ret == true)
        {
            LOG("Successfully parsed map XML file :%s", mapFileName.c_str());
            LOG("width : %d height : %d", mapData.width, mapData.height);
            LOG("tile_width : %d tile_height : %d", mapData.tilewidth, mapData.tileheight);

            LOG("Tilesets----");

            for(const auto& tileset : mapData.tilesets)
			{
				LOG("name : %s firstgid : %d", tileset->name.c_str(), tileset->firstgid);
				LOG("tile width : %d tile height : %d", tileset->tilewidth, tileset->tileheight);
				LOG("spacing : %d margin : %d", tileset->spacing, tileset->margin);
			}
            LOG("Layers----");

            for (const auto& layer : mapData.layers) {
                LOG("id : %d name : %s", layer->id, layer->name.c_str());
				LOG("Layer width : %d Layer height : %d", layer->width, layer->height);
            }
        }

        if (mapFileXML) mapFileXML.reset();
    }

    mapLoaded = ret;
    return ret;
}

// L06: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
Vector2D Map::MapToWorld(int x, int y) const
{
    Vector2D ret;

    ret.setX(x * mapData.tilewidth);
    ret.setY(y * mapData.tileheight);

    return ret;
}


