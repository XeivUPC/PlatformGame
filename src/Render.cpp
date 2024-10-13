#include "Engine.h"
#include "Window.h"
#include "Render.h"
#include "Scene.h"
#include "LevelManager.h"
#include "Log.h"

#define VSYNC true

Render::Render() : Module()
{
	name = "render";
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::Awake()
{
	LOG("Create SDL rendering context");
	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	//flags |= SDL_RENDERER_PRESENTVSYNC;
	LOG("Using vsync");

	SDL_Window* window = Engine::GetInstance().window
		
		
		->window;
	renderer = SDL_CreateRenderer(window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		

		SDL_DisplayMode displayMode;
		if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0)
			SDL_Log("No se pudo obtener el modo de pantalla: %s", SDL_GetError());

		int scale = Engine::GetInstance().window->GetScale();
		int windowWidth = Engine::GetInstance().window->width;  
		int windowHeight = Engine::GetInstance().window->height;

		cameraGameOffset = { 0, 16 * 2.0f};

		camera.x = cameraGameOffset.getX()*scale;
		camera.y = cameraGameOffset.getY()*scale;
		
		SDL_RenderSetLogicalSize(renderer, windowWidth * scale, windowHeight * scale);
	}

	return ret;
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);



	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	ConfineCameraBetweenRange(dt);
	
	return true;
}

bool Render::PostUpdate()
{
	FollowPlayer();
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::ConfineCameraBetweenRange(float dt)
{
	float minX =- METERS_TO_PIXELS(minRangeConfinePosition.getX());
	float maxX =- METERS_TO_PIXELS(maxRangeConfinePosition.getX()) + viewport.w;

	float minY = METERS_TO_PIXELS(-minRangeConfinePosition.getY())+32;
	float maxY = minY;

	if (camera.x > minX) {
		camera.x -= cameraSpeed*dt/1000;
		if(abs(camera.x - minX) < 5)
			camera.x = minX;
	}
	if (camera.x < maxX) {
		camera.x += cameraSpeed * dt / 1000;
		if (abs(camera.x - maxX) < 5)
			camera.x = maxX;
	}
	if (camera.y < minY) {
		camera.y += cameraSpeed * dt / 1000;
		if (abs(camera.y - minY) < 5)
			camera.y = minY;
	}
	if (camera.y > maxY) {
		camera.y -= cameraSpeed * dt / 1000;
		if (abs(camera.y - maxY) < 5)
			camera.y = maxY;
	}

	float playerPosX = Engine::GetInstance().scene->player->position.getX();
	float playerPosY = Engine::GetInstance().scene->player->position.getY();

	if (playerPosX > maxRangeConfinePosition.getX())
		Engine::GetInstance().levelManager->GoToNextSection({ 1,0 });
	if (playerPosX < minRangeConfinePosition.getX())
		Engine::GetInstance().levelManager->GoToNextSection({ -1,0 });


	
	if (playerPosY < minRangeConfinePosition.getY())
		Engine::GetInstance().levelManager->GoToNextSection({0, 1 });
	if (playerPosY > maxRangeConfinePosition.getY())
		Engine::GetInstance().levelManager->GoToNextSection({ 0,-1 });
}

void Render::ConfineCameraBetweenRange()
{
	float minX = -METERS_TO_PIXELS(minRangeConfinePosition.getX());
	float maxX = -METERS_TO_PIXELS(maxRangeConfinePosition.getX()) + viewport.w;

	float minY = METERS_TO_PIXELS(-minRangeConfinePosition.getY()) + 32;
	float maxY = minY;


	float targetPos = METERS_TO_PIXELS((-Engine::GetInstance().scene->player->position.getX())) + viewport.w / 2.f;

	camera.x = targetPos;

	if (camera.x > minX) {
		camera.x = minX;		
	}
	if (camera.x < maxX) {
		camera.x = maxX;
	}
	if (camera.y < minY) {
		camera.y = minY;
	}
	if (camera.y > maxY) {
		camera.y = maxY;
	}

	}


void Render::FollowPlayer()
{
	float minX = -METERS_TO_PIXELS_RAW(minRangeConfinePosition.getX());
	float maxX = -METERS_TO_PIXELS_RAW(maxRangeConfinePosition.getX()) + viewport.w;

	float minY = METERS_TO_PIXELS_RAW(-minRangeConfinePosition.getY()) + 32;
	float maxY = minY;

	float targetPos = METERS_TO_PIXELS_RAW((-Engine::GetInstance().scene->player->position.getX())) + viewport.w / 2.f;

	if (targetPos > minX) {
		return;
	}
	if (targetPos < maxX) {
		return;
	}

	camera.x = targetPos;
}

void Render::SetConfinementValues(Vector2D min, Vector2D max) {
	minRangeConfinePosition = min;
	maxRangeConfinePosition = max;

}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}


// Blit to screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, SDL_RendererFlip flipMode, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY) const
{
	bool ret = true;
	int scale = Engine::GetInstance().window->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flipMode) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	int scale = Engine::GetInstance().window->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	int scale = Engine::GetInstance().window->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	int scale = Engine::GetInstance().window->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(int i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x * scale + camera.x) + (int)(radius * cos(i * factor));
		points[i].y = (int)(y * scale + camera.y) + (int)(radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}
