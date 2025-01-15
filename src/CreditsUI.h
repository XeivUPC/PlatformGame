#pragma once
#include "SDL2/SDL.h"
#include "UI.h"

class GuiControlButton;

class CreditsUI : public UI
{
    bool isOpen = false;
public:
    CreditsUI(Module* ModuleAt);
    ~CreditsUI();
    void Update(float dt);
    void Render();
    void CleanUp();

    void SetIfOpen(bool p);
    bool IsOpen();

    SDL_Texture* bg_texture = nullptr;
    GuiControlButton* backButton;
};

