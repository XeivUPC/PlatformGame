#include "Parallax.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"

Parallax::Parallax()
{
    parallaxFactor = 1;
    offset = 0;
}

Parallax::~Parallax()
{

}

bool Parallax::Awake()
{

    return true;
}

bool Parallax::Start()
{
    LoadTexture("Assets/Maps/Clouds_Parallax.png");
    LoadTexture("Assets/Maps/Castle_Parallax.png");
    LoadTexture("Assets/Maps/Trees1.png");
    return true;
}

bool Parallax::PreUpdate()
{
    return true;
}

bool Parallax::Update(float dt)
{
    for (int i = 0; i < count; i++)
    {
        int velocity = parallaxFactor * i / (count-1)*Engine::GetInstance().window->GetScale();
        //Get Texture Size (double as window size)
        int textureWidth, textureHeight;
        Engine::GetInstance().textures->GetSize(ParallaxLayers[i], textureWidth, textureHeight);

        //Get Main Parallax Step
        int parallaxStep = Engine::GetInstance().render->camera.x * velocity / textureWidth;

        //Get Minimum Width
        if (textureWidth + Engine::GetInstance().render->camera.x * velocity < textureWidth * -parallaxStep* Engine::GetInstance().window->GetScale())
            Engine::GetInstance().render->DrawTexture(ParallaxLayers[i], (-Engine::GetInstance().render->camera.x - textureWidth * (parallaxStep)) / Engine::GetInstance().window->GetScale() - velocity * (-Engine::GetInstance().render->camera.x) / Engine::GetInstance().window->GetScale(), 0, SDL_FLIP_NONE);        
        Engine::GetInstance().render->DrawTexture(ParallaxLayers[i], (-Engine::GetInstance().render->camera.x) / Engine::GetInstance().window->GetScale() - velocity * (-Engine::GetInstance().render->camera.x) / Engine::GetInstance().window->GetScale(), 0, SDL_FLIP_NONE);
    }
    return true;
}



bool Parallax::PostUpdate()
{

    return true;
}

bool Parallax::CleanUp()
{
    for (int i = 0; i < ParallaxLayers.size(); i++)
    {
        Engine::GetInstance().textures->UnLoad(ParallaxLayers[i]);
    }
    ParallaxLayers.clear();
    return true;
}

void Parallax::LoadTexture(std::string path)
{
    SDL_Texture* texture = (Engine::GetInstance().textures->Load(path.c_str()));
    ParallaxLayers.push_back(texture);
    count++;
}