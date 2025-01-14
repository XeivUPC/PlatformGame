#include "Engine.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Log.h"
#include "Window.h"
#include "Box2DFactory.h"
#include "CollisionsManager.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Physics.h"
//#include "Scene.h"
#include "GameScene.h"
#include "IntroScene.h"
#include "TitleScene.h"
#include "EntityManager.h"
#include "Debug.h"
#include "LevelManager.h"
#include "Parallax.h"
#include "TextGenerator.h"
#include "GuiManager.h"
#include "PathfindingSystem.h"

#include "tracy/Tracy.hpp"


// Constructor
Engine::Engine() {
    
	LOG("Constructor Engine::Engine");

    //Measure the amount of ms that takes to execute the App constructor and LOG the result
    Timer timer = Timer();
    startupTime = Timer();
    frameTime = PerfTimer();
    lastSecFrameTime = PerfTimer();
    frames = 0;

    // Modules
    window = std::make_shared<Window>();
    input = std::make_shared<Input>();
    render = std::make_shared<Render>();
    textures = std::make_shared<Textures>();
    audio = std::make_shared<Audio>();
    physics = std::make_shared<Physics>();

    //scene = std::make_shared<Scene>();
    game_scene = std::make_shared<GameScene>(false);
    title_scene = std::make_shared<TitleScene>(false);
    intro_scene = std::make_shared<IntroScene>();

    levelManager = std::make_shared<LevelManager>();
    entityManager = std::make_shared<EntityManager>();
    parallax = std::make_shared<Parallax>();
    text = std::make_shared<TextGenerator>();
    pathfinding = std::make_shared<PathfindingSystem>();

    collisionsManager = std::make_shared<CollisionsManager>();
    ui = std::make_shared<GuiManager>();
    debug = std::make_shared<Debug>();

    // Ordered for awake / Start / Update
    // Reverse order of CleanUp
    AddModule(std::static_pointer_cast<Module>(window));
    AddModule(std::static_pointer_cast<Module>(input));
    AddModule(std::static_pointer_cast<Module>(textures));
    AddModule(std::static_pointer_cast<Module>(audio));
    AddModule(std::static_pointer_cast<Module>(physics));

    //AddModule(std::static_pointer_cast<Module>(scene));
    AddModule(std::static_pointer_cast<Module>(game_scene));
    AddModule(std::static_pointer_cast<Module>(intro_scene));
    AddModule(std::static_pointer_cast<Module>(title_scene));

    AddModule(std::static_pointer_cast<Module>(parallax));
    AddModule(std::static_pointer_cast<Module>(pathfinding));
    //// Add the map module
    AddModule(std::static_pointer_cast<Module>(levelManager));
    AddModule(std::static_pointer_cast<Module>(text));
    //// Add the entity manager
    AddModule(std::static_pointer_cast<Module>(entityManager));
    AddModule(std::static_pointer_cast<Module>(debug));
    AddModule(std::static_pointer_cast<Module>(ui));

    // Render last 
    AddModule(std::static_pointer_cast<Module>(render));

    LOG("Timer App Constructor: %f", timer.ReadMSec());
}

// Static method to get the instance of the Engine class, following the singletn pattern
Engine& Engine::GetInstance() {
    static Engine instance; // Guaranteed to be destroyed and instantiated on first use
    return instance;
}

void Engine::AddModule(std::shared_ptr<Module> module){
    moduleList.push_back(module);
}

// Called before render is available
bool Engine::Awake() {

    //Measure the amount of ms that takes to execute the Awake and LOG the result
    Timer timer = Timer();

    LOG("Engine::Awake");

    bool result = LoadConfig();

    if (result == true)
    {
        // Read the title from the config file and set the windows title 
        // replace the inital string from the value of the title in the config file
        // also read maxFrameDuration 
        gameTitle = configFile.child("config").child("app").child("title").child_value();
        window->SetTitle(gameTitle.c_str());
        maxFrameDuration = configFile.child("config").child("app").child("maxFrameDuration").attribute("value").as_int();

        //Iterates the module list and calls Awake on each module
        bool result = true;
        for (const auto& module : moduleList) {
            result = module
                
                
                ->LoadParameters(configFile.child("config").child(module->name.c_str()));
            if(result) result = module->Awake();
            if (!result) {
                break;
            }
        }
    }

    LOG("Timer App Awake(): %f", timer.ReadMSec());

    return result;
}

// Called before the first frame
bool Engine::Start() {

    //Measure the amount of ms that takes to execute the App Start() and LOG the result
    Timer timer = Timer();

    LOG("Engine::Start");

    //Iterates the module list and calls Start on each module
    bool result = true;
    for (const auto& module : moduleList) {
        if (module->active)
            result = module->Start();
        if (!result) {
            break;
        }
    }

    LOG("Timer App Start(): %f", timer.ReadMSec());

    return result;
}

// Called each loop iteration
bool Engine::Update() {

    ZoneScoped;

    bool ret = true;
    PrepareUpdate();

    if (input->GetWindowEvent(WE_QUIT) == true)
        ret = false;

    if (ret == true)
        ret = PreUpdate();

    if (ret == true)
        ret = DoUpdate();

    if (ret == true)
        ret = PostUpdate();

    FinishUpdate();

    FrameMark;
    return ret;
}

// Called before quitting
bool Engine::CleanUp() {

    //Measure the amount of ms that takes to execute the App CleanUp() and LOG the result
    Timer timer = Timer();

    LOG("Engine::CleanUp");

    //Iterates the module list and calls CleanUp on each module
    bool result = true;
    for (const auto& module : moduleList) {
        if (module->active)
            result = module->CleanUp();
        if (!result) {
            break;
        }
    }

    LOG("Timer App CleanUp(): %f", timer.ReadMSec());

    return result;
}

float Engine::GetDt() const
{
    return dt;
}

int Engine::GetMaxFrameDuration() const
{
    return maxFrameDuration;
}

void Engine::SetMaxFrameDuration(int value)
{
    maxFrameDuration = value;
}

// ---------------------------------------------
void Engine::PrepareUpdate()
{
    ZoneScoped;
    frameTime.Start();
}

// ---------------------------------------------
void Engine::FinishUpdate()
{
    ZoneScoped;
    //Cap the framerate of the gameloop
    double currentDt = frameTime.ReadMs();
    if (maxFrameDuration > 0 && currentDt < maxFrameDuration) {
        int delay = (int)(maxFrameDuration - currentDt);

        PerfTimer delayTimer = PerfTimer();
        SDL_Delay(delay);
        //Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected
        //LOG("We waited for %I32u ms and got back in %f ms",delay,delayTimer.ReadMs());
    }

    // Amount of frames since startup
    frameCount++;

    // Amount of time since game start (use a low resolution timer)
    secondsSinceStartup = startupTime.ReadSec();

    // Amount of ms took the last update (dt)
    dt = (float)frameTime.ReadMs();

    // Amount of frames during the last second
    lastSecFrameCount++;

    // Average FPS for the whole game life
    if (lastSecFrameTime.ReadMs() > 1000) {
        lastSecFrameTime.Start();
        averageFps = (averageFps + lastSecFrameCount) / 2;
        framesPerSecond = lastSecFrameCount;
        lastSecFrameCount = 0;
    }

    // Shows the time measurements in the window title
    // check sprintf formats here https://cplusplus.com/reference/cstdio/printf/
    std::stringstream ss;
    ss << gameTitle << ": Av.FPS: " << std::fixed << std::setprecision(2) << averageFps
        << " Last sec frames: " << framesPerSecond
        << " Last dt: " << std::fixed << std::setprecision(3) << dt
        << " Time since startup: " << secondsSinceStartup
        << " Frame Count: " << frameCount;

    std::string titleStr = ss.str();

    window->SetTitle(titleStr.c_str());
}

// Call modules before each loop iteration
bool Engine::PreUpdate()
{
    ZoneScoped;
    //Iterates the module list and calls PreUpdate on each module
    bool result = true;
    for (const auto& module : moduleList) {
        if(module->active)
            result = module->PreUpdate();
        if (!result) {
            break;
        }
    }

    return result;
}

// Call modules on each loop iteration
bool Engine::DoUpdate()
{
    ZoneScoped;
    //Iterates the module list and calls Update on each module
    bool result = true;
    for (const auto& module : moduleList) {
        if (module->active)
            result = module->Update(dt);
        if (!result) {
            break;
        }
    }

    return result;
}

// Call modules after each loop iteration
bool Engine::PostUpdate()
{
    //Iterates the module list and calls PostUpdate on each module
    bool result = true;
    for (const auto& module : moduleList) {
        if (module->active)
            result = module->PostUpdate();
        if (!result) {
            break;
        }
    }

    return result;
}

bool Engine::LoadConfig()
{
    bool ret = true;

    //Load config.xml file using load_file() method from the xml_document class
    // If the result is ok get the main node of the XML
    // else, log the error
    // check https://pugixml.org/docs/quickstart.html#loading

    pugi::xml_parse_result result = configFile.load_file("config.xml");
    if (result)
    {
        LOG("config.xml parsed without errors");
    }
    else
    {
        LOG("Error loading config.xml: %s", result.description());
    }

    return ret;
}
