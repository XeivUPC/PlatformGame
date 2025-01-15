#include "SDL2/SDL.h"
#include "UI.h"

class GuiControlButton;

class TitleUI : public UI
{
private:
    int shovel_y = -300;

public:
    TitleUI(Module* ModuleAt);
    ~TitleUI();
    void Update(float dt);
    void Render();
    void CleanUp();

    SDL_Texture* bg_texture = nullptr;
    GuiControlButton* playButton;
    GuiControlButton* continueButton;
    GuiControlButton* settingsButton;
    GuiControlButton* creditsButton;
    GuiControlButton* exitButton;
};