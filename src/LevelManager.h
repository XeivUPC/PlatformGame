#pragma once

#include "Module.h"
#include "LevelSection.h"
#include "CheckPoint.h"
#include <unordered_map>
#include <vector>

class LevelManager : public Module
{
public:
    LevelManager();

    // Destructor
    virtual ~LevelManager();

    // Called before render is available
    bool Awake();

    bool LoadParameters(xml_node parameters);

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool Update(float dt);
    bool PostUpdate();

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool LoadLevel(int levelToPlay);
    bool ChargeAllLevelSection(int startingIndex);
    bool ChargeAdjacentSections(LevelSection* mainSection);

    bool LoadSection(int sectionNumber);
    void GoToNextSection(b2Vec2 direction);

    void GoToClosestCheckPoint();
    Vector2D GetClosestCheckPointPosition();
    void RegisterCheckPoint(CheckPoint* checkPoint);

    void LoadSaveFile(std::string path);
    void SaveSaveFile(std::string path);

    LevelSection* GetCurrentSection();

    std::string path;
    std::string levelsPath;
    std::string texturePath;

    bool levelLoaded = false;


private:

    Vector2D spawnPoint;

    std::unordered_map<int, LevelSection*> loadedSections;
    std::vector<int> sectionsInUse;
    std::vector<CheckPoint*> checkPoints;

    int currentSection=0;
    int currentLevel=-1;

    xml_node parameters;


    int saveSoundId;

};

