#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "SString.h"

#include "PugiXml/src/pugixml.hpp"


#define NUM_MODULES 14

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleAudio;
class ModulePlayer;
class SceneIntro;
class SceneLevel1;
class ModuleParticles;
class ModuleCollisions;
class ModuleEnemies;
class ModuleFadeToBlack;
class ModuleFonts;
class ModuleRender;
class ModulePhysics;

class Application
{

public:

	//Constructor. Creates all necessary modules for the application
	Application();

	//Destructor. Removes all module objects
	~Application();

	// Called before render is available
	bool Awake();

	//Initializes all modules
	bool Init();

	//Updates all modules (PreUpdate, Update and PostUpdate)
	Update_Status Update();

	//Releases all the application data
	bool CleanUp();

	const char* GetTitle() const;

public:
	// An array to store all modules
	Module* modules[NUM_MODULES];

	// All the modules stored individually
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleAudio* audio = nullptr;

	ModulePlayer* player = nullptr;

	SceneIntro* sceneIntro = nullptr;
	SceneLevel1* sceneLevel_1 = nullptr;

	ModuleEnemies* enemies = nullptr;
	ModuleParticles* particles = nullptr;

	ModuleCollisions* collisions = nullptr;
	ModuleFadeToBlack* fade = nullptr;
	ModuleFonts* fonts = nullptr;

	ModuleRender* render = nullptr;

	ModulePhysics* physics = nullptr;

private:

	float avFPS = 0.0f;
	uint seconds;
	SString gameTitle;
	SString organization;

	List<Module*> modules;

	//Load config file
	bool LoadConfig();


	//xml_document to store the config file
	pugi::xml_document configFile;

	uint frames;
	float dt;

	//Required variables to calculate timing measures
	Timer startupTime;
	PerfTimer frameTime;
	PerfTimer lastSecFrameTime;

	uint frameCount = 0;
	uint framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	uint32 secondsSinceStartup = 0;

	//Consultar - Set max frame duration
	uint32 maxFrameDuration = 16;
};

// Global var made extern for Application ---
extern Application* App;

#endif // __APPLICATION_H__