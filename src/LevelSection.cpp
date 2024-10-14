#include "LevelSection.h"
#include "LOG.h"
#include "Engine.h"
#include "Textures.h"
#include "Scene.h"
#include <sstream> 
#include "Render.h"
#include "Box2DCreator.h"
#include "CheckPoint.h"
#include "DirtBlock.h"
#include "EntityManager.h"
#include "MovingPlatform.h"

LevelSection::LevelSection()
{

}

LevelSection::~LevelSection()
{
}

bool LevelSection::Update(float dt)
{

    for (const auto& mapLayer : mapData.layers) {

        for (int i = 0; i < mapData.width; i++) {
            for (int j = 0; j < mapData.height; j++) {
                //Get the gid from tile
                int gid = mapLayer->Get(i, j) -1;
                if (gid < 0)
                    continue;
                //Get the Rect from the tileSetTexture;
                SDL_Rect tileRect = mapData.tilesets.front()->GetRect(gid);
                //Get the screen coordinates from the tile coordinates
                Vector2D mapCoord = MapToWorld(i, j);

                // Complete the draw function
                Engine::GetInstance().render->DrawTexture(mapData.tilesets.front()->texture, mapCoord.getX() + sectionOffset.x , mapCoord.getY() + sectionOffset.y, SDL_FLIP_NONE, &tileRect);

            }
        }
    }

    for (auto*& collider : colliders)
    {
        //Engine::GetInstance().box2DCreator->RenderBody(collider, b2Color{ 0,255,0,255 });
    }

	return true;
}

bool LevelSection::CleanUp()
{
    // Make sure you clean up any memory allocated from tilesets/map
    for (const auto& tileset : mapData.tilesets) {
        Engine::GetInstance().textures->UnLoad(tileset->texture);
        delete tileset;
    }
    mapData.tilesets.clear();

    // clean up all layer data
    for (const auto& layer : mapData.layers)
    {
        delete layer->tiles;
        delete layer;
    }
    mapData.layers.clear();

    for (auto*& collider : colliders)
    {
        Engine::GetInstance().scene->world->DestroyBody(collider);
    }

    return true;
}


bool LevelSection::Load(std::string fileName, std::string texturePath, b2Vec2 offset, bool loadColliders, bool loadObjects)
{

    bool ret = true;

    // L05: DONE 3: Implement LoadMap to load the map properties
    // retrieve the paremeters of the <map> node and save it into map data
;
    pugi::xml_parse_result result = mapFileXML.load_file(fileName.c_str());

    if (result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", fileName.c_str(), result.description());
        ret = false;
    }
    else {

        sectionOffset = offset;
        pugi::xml_node mapProperties = mapFileXML.child("map").child("properties").find_child_by_attribute("property", "name", "BottomSection");
        bottomSection = mapProperties.attribute("value").as_int();

        mapProperties = mapFileXML.child("map").child("properties").find_child_by_attribute("property", "name", "Level");
        sectionNumber = mapProperties.attribute("value").as_int();
        mapProperties = mapFileXML.child("map").child("properties").find_child_by_attribute("property", "name", "TopSection");
        topSection = mapProperties.attribute("value").as_int();
        mapProperties = mapFileXML.child("map").child("properties").find_child_by_attribute("property", "name", "LeftSection");
        leftSection = mapProperties.attribute("value").as_int();
        mapProperties = mapFileXML.child("map").child("properties").find_child_by_attribute("property", "name", "RightSection");
        rightSection = mapProperties.attribute("value").as_int();


        CreateMapData(&mapFileXML);
        for (pugi::xml_node tilesetNode = mapFileXML.child("map").child("tileset"); tilesetNode != NULL; tilesetNode = tilesetNode.next_sibling("tileset")) {
            TileSet* tileset = CreateTileset(&tilesetNode, texturePath);
            mapData.tilesets.push_back(tileset);
        }
        for (pugi::xml_node layerNode = mapFileXML.child("map").child("layer"); layerNode != NULL; layerNode = layerNode.next_sibling("layer")) {
            MapLayer* mapLayer = CreateMapLayer(&layerNode);
            mapData.layers.push_back(mapLayer);
        }

        mapNode = mapFileXML.child("map");

        if (loadColliders)
            LoadColliders();
        if (loadObjects)
            LoadObjects();

    }
    return ret;
}

Vector2D LevelSection::MapToWorld(int x, int y) const
{
    Vector2D ret;

    ret.setX(x * mapData.tilewidth);
    ret.setY(y * mapData.tileheight);

    return ret;
}

TileSet* LevelSection::CreateTileset(xml_node* node, std::string texturePath)
{
    TileSet* tileset = new TileSet();

    std::string tsxFileName = node->attribute("source").as_string();
    tsxFileName = tsxFileName.substr(3);

    std::string tsxFilePath = texturePath + tsxFileName;
    pugi::xml_document document;
    pugi::xml_parse_result result2 = document.load_file(tsxFilePath.c_str());

    tileset->name = document.child("tileset").attribute("name").as_string();
    tileset->firstgid = document.child("tileset").attribute("firstgid").as_int();
    tileset->tilewidth = document.child("tileset").attribute("tilewidth").as_int();
    tileset->tileheight = document.child("tileset").attribute("tileheight").as_int();
    tileset->columns = document.child("tileset").attribute("columns").as_int();

    std::string mapTex = texturePath;
    mapTex += document.child("tileset").child("image").attribute("source").as_string();
    tileset->texture = Engine::GetInstance().textures->Load(mapTex.c_str());

    if (document)
        document.reset();

    return tileset;
}

MapLayer* LevelSection::CreateMapLayer(xml_node* node)
{
    MapLayer* mapLayer = new MapLayer();

    mapLayer->id = node->attribute("id").as_int();
    mapLayer->name = node->attribute("name").as_string();
    mapLayer->width = node->attribute("width").as_int();
    mapLayer->height = node->attribute("height").as_int();

    //Reserve the memory for the data 
    mapLayer->tiles = new unsigned int[mapLayer->width * mapLayer->height];
    memset(mapLayer->tiles, 0, mapLayer->width * mapLayer->height);

    //Iterate over all the tiles and assign the values in the data array

    std::string tileData = node->child("data").text().as_string();
    std::stringstream ss(tileData);
    std::string item;

    int i = 0;
    while (std::getline(ss, item, ',')) {
        // Convert string to integer and store in the vector
        mapLayer->tiles[i] = std::stoi(item);
        i++;
    }
    return mapLayer;
}

void LevelSection::CreateMapData(xml_document* document)
{

    mapData.width = document->child("map").attribute("width").as_int();
    mapData.height = document->child("map").attribute("height").as_int();
    mapData.tilewidth = document->child("map").attribute("tilewidth").as_int();
    mapData.tileheight = document->child("map").attribute("tileheight").as_int();
}

void LevelSection::LoadColliders() {

    
    for (pugi::xml_node colliderNode = mapNode.find_child_by_attribute("objectgroup", "name", "Colliders").child("object"); colliderNode != NULL; colliderNode = colliderNode.next_sibling("object")) {

        b2Body* collider = CreateColliders(&colliderNode);
        colliders.push_back(collider);
    }

}

void LevelSection::LoadObjects()
{
    for (pugi::xml_node objectNode = mapNode.find_child_by_attribute("objectgroup", "name", "Objects").child("object"); objectNode != NULL; objectNode = objectNode.next_sibling("object")) {

        pugi::xml_node objectTypePropety = objectNode.child("properties").find_child_by_attribute("property", "name", "Type");

        std::string type = objectTypePropety.attribute("value").as_string();
        if (type  == "CheckPoint") {
            float x = objectNode.attribute("x").as_int();
            float y = objectNode.attribute("y").as_int();

            Vector2D postion{ PIXEL_TO_METERS(x) + PIXEL_TO_METERS(sectionOffset.x), PIXEL_TO_METERS(y) + PIXEL_TO_METERS(sectionOffset.y) };
            CheckPoint* checkPoint = new CheckPoint(sectionNumber, postion);
            Engine::GetInstance().entityManager->AddEntity((Entity*)checkPoint);
        }

        if (type == "DirtBlock") {
            float x = objectNode.attribute("x").as_int();
            float y = objectNode.attribute("y").as_int();

            pugi::xml_node objectSizePropety = objectNode.child("properties").find_child_by_attribute("property", "name", "Size");
            int size = objectSizePropety.attribute("value").as_int();

            Vector2D postion{ PIXEL_TO_METERS(x) + PIXEL_TO_METERS(sectionOffset.x), PIXEL_TO_METERS(y) + PIXEL_TO_METERS(sectionOffset.y) };
            DirtBlock* dirtBlock = new DirtBlock((DirtBlock::DirtSize)size, postion);
            Engine::GetInstance().entityManager->AddEntity((Entity*)dirtBlock);
        }

        if (type == "BubbleGenerator") {
            float x = objectNode.attribute("x").as_int();
            float y = objectNode.attribute("y").as_int();
        }

        if (type == "MovingPlatform") {
            float x = objectNode.attribute("x").as_int();
            float y = objectNode.attribute("y").as_int();

            int width = objectNode.attribute("width").as_int();
            int height = objectNode.attribute("height").as_int();

            pugi::xml_node objectMoveTypeProperty = objectNode.child("properties").find_child_by_attribute("property", "name", "IsVertical");
            bool isVertical = objectMoveTypeProperty.attribute("value").as_bool();
            
            pugi::xml_node platformTypePropety = objectNode.child("properties").find_child_by_attribute("property", "name", "PlatformType");
            int platformType = platformTypePropety.attribute("value").as_bool();



            Vector2D leftSide{};
            Vector2D rightSide{};

            if (!isVertical) {
                leftSide = { PIXEL_TO_METERS((x + mapData.tilewidth/2)) + PIXEL_TO_METERS(sectionOffset.x),PIXEL_TO_METERS((y + height / 2)) + PIXEL_TO_METERS(sectionOffset.y) };
                rightSide ={ PIXEL_TO_METERS((x + width - mapData.tilewidth/2)) + PIXEL_TO_METERS(sectionOffset.x), PIXEL_TO_METERS((y + height / 2)) + PIXEL_TO_METERS(sectionOffset.y) };
            }
            else {
                leftSide = { PIXEL_TO_METERS((x + width/2)) + PIXEL_TO_METERS(sectionOffset.x),PIXEL_TO_METERS((y + height - mapData.tileheight/2)) + PIXEL_TO_METERS(sectionOffset.y) };
                rightSide = { PIXEL_TO_METERS((x + width/2)) + PIXEL_TO_METERS(sectionOffset.x), PIXEL_TO_METERS((y + mapData.tileheight/2)) + PIXEL_TO_METERS(sectionOffset.y) };
            }

           

            MovingPlatform* movingPlatform = new MovingPlatform(sectionNumber, leftSide, rightSide, platformType, isVertical);
            Engine::GetInstance().entityManager->AddEntity((Entity*)movingPlatform);
        }
    }
}

b2Body* LevelSection::CreateColliders(xml_node* node)
{
    b2World* world = Engine::GetInstance().scene->world;

    int x = node->attribute("x").as_int();
    int y = node->attribute("y").as_int();

    int width = node->attribute("width").as_int();
    int height = node->attribute("height").as_int();

    x += width / 2;
    y += height / 2;

    b2Vec2 position{ PIXEL_TO_METERS(x) + PIXEL_TO_METERS(sectionOffset.x), PIXEL_TO_METERS(y) + PIXEL_TO_METERS(sectionOffset.y) };

    b2Filter filter;
    for (pugi::xml_node colliderProperties = node->child("properties"); colliderProperties != NULL; colliderProperties = colliderProperties.next_sibling("properties"))
    {

        pugi::xml_node layersObjectProperty = colliderProperties.find_child_by_attribute("property", "name", "OBJECT_LAYERS");
        std::string value = layersObjectProperty.attribute("value").as_string();
        AddLayers(&filter.categoryBits, value);

        pugi::xml_node layersAffectingProperty = colliderProperties.find_child_by_attribute("property", "name", "AFFECTING_LAYERS");
        value = layersAffectingProperty.attribute("value").as_string();
        AddLayers(&filter.maskBits, value);


    }
    b2Body* collider = Engine::GetInstance().box2DCreator->CreateBox(world, position, PIXEL_TO_METERS(width), PIXEL_TO_METERS(height));

    collider->SetType(b2_staticBody);
    collider->GetFixtureList()[0].SetFilterData(filter);

    return collider;
}

void LevelSection::AddLayers(uint16* container, std::string layersInput)
{
    std::istringstream stream(layersInput);
    std::string line;
    uint16 layersToAdd = 0;
    while (std::getline(stream, line)) {

        if (line == "PLAYER_LAYER")
            layersToAdd |= Engine::GetInstance().PLAYER_LAYER;
        if(line == "GROUND_LAYER")
            layersToAdd |= Engine::GetInstance().GROUND_LAYER;
        if (line == "ENEMY_LAYER")
            layersToAdd |= Engine::GetInstance().ENEMY_LAYER;
        if (line == "PLAYER_ATTACK_LAYER")
            layersToAdd |= Engine::GetInstance().PLAYER_ATTACK_LAYER;
        if (line == "LADDER_LAYER")
            layersToAdd |= Engine::GetInstance().LADDER_LAYER;

        if (line == "USE_LADDER_LAYER")
            layersToAdd |= Engine::GetInstance().USE_LADDER_LAYER;
    }
    *container = layersToAdd;
}


