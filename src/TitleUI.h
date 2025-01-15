#include "SDL2/SDL.h"
#include "UI.h"

class GuiControlButton;
class TitleUI : public UI
{
private:
    int shovel_y = 0;

public:
    TitleUI(Module* ModuleAt);
    ~TitleUI();
    void Update(float dt);
    void Render();
    void CleanUp();
    GuiControlButton* playButton;
    GuiControlButton* continueButton;
    GuiControlButton* settingsButton;
    GuiControlButton* creditsButton;
    GuiControlButton* exitButton;
};