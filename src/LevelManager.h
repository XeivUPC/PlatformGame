#pragma once

#include "Module.h"
#include "LevelSection.h"
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

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool LoadLevel(int levelToPlay);
    bool ChargeAllLevelSection(int startingIndex);
    bool ChargeAdjacentSections(LevelSection* mainSection);

    bool LoadSection(int sectionNumber);
    void GoToNextSection(b2Vec2 direction);

    LevelSection* GetCurrentSection();

    std::string path;
    std::string levelsPath;
    std::string texturePath;

private:

    std::unordered_map<int, LevelSection*> loadedSections;
    std::vector<int> sectionsInUse;

    int currentSection=0;
    int currentLevel=0;

    xml_node parameters;

};

