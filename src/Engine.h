#pragma once

#include <memory>
#include <list>
#include "Module.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "pugixml.hpp"

using namespace pugi;

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Physics;

class GameScene;
class IntroScene;
class TitleScene;
class EntityManager;
class LevelManager;
class CollisionsManager;
class Parallax;
class TextGenerator;
class GuiManager;
class Debug;
class PathfindingSystem;


#define GRAVITY_X 0.0f
#define GRAVITY_Y -36.0f

#define PIXELS_PER_METER 16.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 1.0f/PIXELS_PER_METER // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * (m)))
#define METERS_TO_PIXELS_RAW(m) (PIXELS_PER_METER * (m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * (p))


class Engine
{
public:

	const uint16_t PLAYER_LAYER = 0x0001;
	const uint16_t GROUND_LAYER =0x0002;
	const uint16_t PLAYER_ATTACK_LAYER= 0x0004;
	const uint16_t ENEMY_LAYER =0x0008;
	const uint16_t ENEMY_ATTACK_LAYER =0x0080;
	const uint16_t LADDER_LAYER =0x0010;
	const uint16_t USE_LADDER_LAYER =0x0020;
	const uint16_t INTERACTABLE_LAYER =0x0040;

	// Public method to get the instance of the Singleton
	static Engine& GetInstance();

	//	
	void AddModule(std::shared_ptr<Module> module);

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	float GetDt() const;

	int GetMaxFrameDuration() const;
	void SetMaxFrameDuration(int value);

private:

	// Private constructor to prevent instantiation
	// Constructor
	Engine();

	// Delete copy constructor and assignment operator to prevent copying
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load config file
	bool LoadConfig();

	std::list<std::shared_ptr<Module>> moduleList;

public:

	enum EngineState
	{
		CREATE = 1,
		AWAKE,
		START,
		LOOP,
		CLEAN,
		FAIL,
		EXIT
	};

	// Modules
	std::shared_ptr<Window> window;
	std::shared_ptr<Input> input;
	std::shared_ptr<Render> render;
	std::shared_ptr<Textures> textures;
	std::shared_ptr<Audio> audio;
	std::shared_ptr<Physics> physics;
	std::shared_ptr<GameScene> game_scene;
	std::shared_ptr<IntroScene> intro_scene;
	std::shared_ptr<TitleScene> title_scene;
	std::shared_ptr<EntityManager> entityManager;
	std::shared_ptr<LevelManager> levelManager;
	std::shared_ptr<CollisionsManager> collisionsManager;
	std::shared_ptr<Parallax> parallax;
	std::shared_ptr<TextGenerator> text;
	std::shared_ptr<GuiManager> ui;
	std::shared_ptr<Debug> debug;
	std::shared_ptr<PathfindingSystem> pathfinding;

private: 

	// Delta time
	float dt; 
	//Frames since startup
	int frames;

	// Calculate timing measures
	// required variables are provided:
	Timer startupTime;
	PerfTimer frameTime;
	PerfTimer lastSecFrameTime;

	int frameCount = 0;
	int framesPerSecond = 0;
	int lastSecFrameCount = 0;

	float averageFps = 0.0f;
	int secondsSinceStartup = 0;

	//Maximun frame duration in miliseconds.
	int maxFrameDuration = 16;

	std::string gameTitle = "Platformer Game";

	// Variable to load and store the XML file in memory
	xml_document configFile;
};