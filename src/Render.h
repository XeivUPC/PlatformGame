#pragma once

#include "Module.h"
#include "Vector2D.h"
#include "SDL2/SDL.h"
#include <unordered_map>

class Render : public Module
{


public:
	enum RenderLayers
	{
		Layer0,
		Layer1,
		Layer2,
		Layer3,
		Enemy,
		Layer4,
		Layer5,
		Layer6,
		Layer7,


		////////
		LAST_DONT_USE
	};

	Render();

	// Destructor
	virtual ~Render();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();

	// Drawing
	bool DrawTexture(SDL_Texture* texture, int x, int y, SDL_RendererFlip flipMode, const SDL_Rect* section = NULL, SDL_Color color = { 255,255,255,255 }, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX) const;
	bool DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool useCamera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;

	// Layers
	void SelectLayer(RenderLayers layerIndex);
	void DeselectLayer() const;

	void CreateLayer(RenderLayers layerIndex);
	void CleanLayer(RenderLayers layerIndex);
	void RenderLayer(RenderLayers layerIndex);

	void LockLayer(RenderLayers layerIndex);
	void UnlockLayer();

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	void SetConfinementValues(Vector2D min, Vector2D max);
	void ConfineCameraBetweenRange();

public:

	Vector2D cameraGameOffset;
	SDL_Renderer* renderer;
	SDL_Rect camera;
	SDL_Rect viewport;
	SDL_Color background;

private:
	void ConfineCameraBetweenRange(float dt);
	void FollowPlayer();

	float cameraSpeed = 600;
	Vector2D minRangeConfinePosition{ 0,0 };
	Vector2D maxRangeConfinePosition{ 100,0 };


	std::unordered_map<RenderLayers, SDL_Texture*> layers;
	bool isLayerBlocked = false;
};