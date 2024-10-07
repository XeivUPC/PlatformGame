#include "Parallax.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"

Parallax::Parallax()
{
    ParallaxFactor = 2;
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
        // Obtener el tamaño de la textura
        int textureWidth = 0;
        int textureHeight = 0;
        Engine::GetInstance().textures->GetSize(ParallaxLayers[0], textureWidth, textureHeight);
        SDL_Rect rect{ 0 ,0 ,Engine::GetInstance().window->width, Engine::GetInstance().window->height };
        if (Engine::GetInstance().window->width - Engine::GetInstance().render->camera.x > textureWidth)
        {
            Engine::GetInstance().render->DrawTexture(ParallaxLayers[0], Engine::GetInstance().window->width - Engine::GetInstance().render->camera.x-textureWidth, 0, SDL_FLIP_NONE, &rect);
        }
        
        Engine::GetInstance().render->DrawTexture(ParallaxLayers[0], 0, 0, SDL_FLIP_NONE ,&rect);
    //    // Calcular el desplazamiento en x
    //    int offsetX = -Engine::GetInstance().render->camera.x * i / ParallaxFactor;

    //    // Calcular cuántas texturas necesitamos dibujar
    //    int startX = offsetX;
    //    if (startX < Engine::GetInstance().window->width)
    //    {
    //        SDL_Rect rect = { startX, 0, textureWidth, textureHeight };
    //        Engine::GetInstance().render->DrawTexture(ParallaxLayers[i], startX / Engine::GetInstance().window->GetScale(), 0, SDL_FLIP_NONE, &rect);
    //        startX += textureWidth; // Mover a la siguiente posición de la textura
    //    }

    //    // Dibujo adicional para cubrir el lado izquierdo de la pantalla
    //    startX = offsetX - textureWidth; // Empezar desde la izquierda
    //    if (startX > -textureWidth) // Dibuja hasta que se salga por la izquierda
    //    {
    //        SDL_Rect rect = { startX, 0, textureWidth, textureHeight };
    //        Engine::GetInstance().render->DrawTexture(ParallaxLayers[i], startX / Engine::GetInstance().window->GetScale(), 0, SDL_FLIP_NONE, &rect);
    //        startX -= textureWidth; // Mover a la siguiente posición de la textura
    //    }
    //}
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