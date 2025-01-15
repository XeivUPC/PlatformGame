#include "Parallax.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include <string>

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
    return true;
}

bool Parallax::PreUpdate()
{
    return true;
}

bool Parallax::Update(float d)
{
    int textureWidth;
    int textureHeight;

    Vector2D cameraOffset = Engine::GetInstance().render->cameraGameOffset;

    float cameraX = -Engine::GetInstance().render->camera.x + cameraOffset.getX();
    float cameraY = -Engine::GetInstance().render->camera.y + cameraOffset.getY();

    
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
        
        Engine::GetInstance().render->LockLayer(Render::RenderLayers::Layer0);
        Engine::GetInstance().render->DrawTexture(ParallaxLayers[i].texture, cameraX + ParallaxLayers[i].offset.getX(), cameraY, SDL_FLIP_NONE, &rect);
        if (ParallaxLayers[i].speed != 0) {
            Engine::GetInstance().render->DrawTexture(ParallaxLayers[i].texture, cameraX + ParallaxLayers[i].offset.getX() - textureWidth, cameraY, SDL_FLIP_NONE, &rect);
            Engine::GetInstance().render->DrawTexture(ParallaxLayers[i].texture, cameraX + ParallaxLayers[i].offset.getX() + textureWidth, cameraY, SDL_FLIP_NONE, &rect);
        }
        Engine::GetInstance().render->UnlockLayer();
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
    ParallaxLayers.clear();
    return true;
}

void Parallax::LoadTexture(std::string path, float speed)
{
    SDL_Texture* texture = (Engine::GetInstance().textures->GetTexture(path.c_str()));
    ParallaxLayers.push_back({ texture, speed , {0,0} });
}

float Parallax::ReloadWithLevel(int level)
{
    CleanUp();
    LoadTexture("CloudsParallax"+std::to_string(level), 0);
    LoadTexture("CastleParallax" + std::to_string(level), 0.2);
    LoadTexture("Trees1Parallax" + std::to_string(level), 0.4);
    LoadTexture("Trees2Parallax" + std::to_string(level), 0.6);
    return 0.0f;
}
