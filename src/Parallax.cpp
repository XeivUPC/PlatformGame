#include "Parallax.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"

Parallax::Parallax()
{
    ParallaxFactor = 1;
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
    return true;
}

bool Parallax::PostUpdate()
{
    for (int i = 0; i < count; i++)
    {
        SDL_Rect rect = SDL_Rect{(int)(i/count*ParallaxFactor+offset+Engine::GetInstance().render->camera.x), 0, Engine::GetInstance().render->camera.w, Engine::GetInstance().render->camera.h};
        Engine::GetInstance().render->DrawTexture(ParallaxLayers[i], 0, 0, SDL_FLIP_NONE, &rect);
    }
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