#include "Parallax.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"

Parallax::Parallax()
{

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
    LoadTexture("Assets/Textures/Parallax/Cloud_Parallax.png",0);
    LoadTexture("Assets/Textures/Parallax/Castle_Parallax.png",0.2);
    LoadTexture("Assets/Textures/Parallax/Trees_1_Parallax.png",0.4);
    LoadTexture("Assets/Textures/Parallax/Trees_2_Parallax.png",0.6);
    return true;
}

bool Parallax::PreUpdate()
{
    return true;
}

bool Parallax::Update(float dt)
{
    int textureWidth;
    int textureHeight;

    int scale = Engine::GetInstance().window->GetScale();
    Vector2D cameraOffset = Engine::GetInstance().render->cameraGameOffset;

    float cameraX = -Engine::GetInstance().render->camera.x / (float)scale + cameraOffset.getX();
    float cameraY = -Engine::GetInstance().render->camera.y / (float)scale + cameraOffset.getY();

    
    for (int i = 0; i < ParallaxLayers.size(); i++)
    {
        Engine::GetInstance().textures->GetSize(ParallaxLayers[i].texture, textureWidth, textureHeight);
        SDL_Rect rect = SDL_Rect{ 0, 0, textureWidth,textureHeight };
        if (ParallaxLayers[i].speed != 0) {
            ParallaxLayers[i].offset.setX(ParallaxLayers[i].offset.getX() - (cameraX - lastCameraX) * ParallaxLayers[i].speed);

            if (ParallaxLayers[i].offset.getX() > textureWidth)
                ParallaxLayers[i].offset.setX(0);
            else if (ParallaxLayers[i].offset.getX() < -textureWidth)
                ParallaxLayers[i].offset.setX(0);
        }
  
        Engine::GetInstance().render->DrawTexture(ParallaxLayers[i].texture, cameraX + ParallaxLayers[i].offset.getX(), cameraY, SDL_FLIP_NONE, &rect);
        if (ParallaxLayers[i].speed != 0) {
            Engine::GetInstance().render->DrawTexture(ParallaxLayers[i].texture, cameraX + ParallaxLayers[i].offset.getX() - textureWidth, cameraY, SDL_FLIP_NONE, &rect);
            Engine::GetInstance().render->DrawTexture(ParallaxLayers[i].texture, cameraX + ParallaxLayers[i].offset.getX() + textureWidth, cameraY, SDL_FLIP_NONE, &rect);
        }
    }

    lastCameraX = cameraX;

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
        Engine::GetInstance().textures->UnLoad(ParallaxLayers[i].texture);
    }
    ParallaxLayers.clear();
    return true;
}

void Parallax::LoadTexture(std::string path, float speed)
{
    SDL_Texture* texture = (Engine::GetInstance().textures->Load(path.c_str()));
    ParallaxLayers.push_back({ texture, speed , {0,0} });
}
