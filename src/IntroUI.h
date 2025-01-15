#pragma once
#include "SDL2/SDL.h"
#include "UI.h"

class IntroUI : public UI
{
private:
    int shovel_y = 0;

public:
    IntroUI(Module* ModuleAt);
    ~IntroUI();
    void Update(float dt);
    void Render();
    void CleanUp();

    SDL_Texture* bg_texture = nullptr;

};