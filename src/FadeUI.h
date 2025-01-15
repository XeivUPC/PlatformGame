#pragma once
#include "SDL2/SDL.h"
#include "UI.h"
#include "Timer.h"

class FadeUI : public UI
{

public:
    FadeUI(Module* ModuleAt);
    ~FadeUI();
    void Update(float dt);
    void Render();
    void ForceRender();
    void CleanUp();

    void StartFadeIn(SDL_Color color, float time);
    void StartFadeOut(SDL_Color color, float time);

    bool IsFading();
private:
    void FadeIn();
    void FadeOut();

    void FadeUpdate();
    void FadeRender();

    Timer fadeTimer;
    float fadeTime = -1;

    SDL_Color fadeColor = {255,255,255};
    bool doingFadeIn = false;
    bool doingFadeOut = false;
};

