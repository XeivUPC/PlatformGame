#pragma once
#include "Vector2D.h"

class LevelSection
{
public:
    bool LoadSectionData();

    int GetLeftSectionNumber();
    int GetRightSectionNumber();
    int GetTopSectionNumber();
    int GetBottomSectionNumber();
private:
    Vector2D cameraVerticalOffsetRange{ 0,0 };
    Vector2D cameraHorizontalOffsetRange{ 0,0 };

    int leftSection = -1;
    int rightSection = -1;
    int topSection = -1;
    int bottomSection = -1;
};

