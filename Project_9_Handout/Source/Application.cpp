#include "Application.h"

#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "SceneIntro.h"
#include "SceneLevel1.h"
#include "ModuleParticles.h"
#include "ModuleEnemies.h"
#include "ModuleCollisions.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"

Application::Application()
{

	// The order in which the modules are added is very important.
	// It will define the order in which Pre/Update/Post will be called
	// Render should always be last, as our last action should be updating the screen

	modules[0] =	window =		new ModuleWindow(true);
	modules[1] =	input =			new ModuleInput(true);
	modules[2] =	textures =		new ModuleTextures(true);
	modules[3] =	audio =			new ModuleAudio(true);

	modules[4] =	physics =		new ModulePhysics(false);

	modules[5] =	sceneIntro =	new SceneIntro(true);
	modules[6] =	sceneLevel_1 =	new SceneLevel1(false);		//Gameplay scene starts disabled
	modules[7] =	player =		new ModulePlayer(false);	//Player starts disabled
	modules[8] =	particles =		new ModuleParticles(true);
	modules[9] =	enemies =		new ModuleEnemies(false);	//Enemies start disabled

	modules[10] =	collisions =	new ModuleCollisions(true);
	modules[11] =	fade =			new ModuleFadeToBlack(true);
	modules[12] =	render =		new ModuleRender(true);
	modules[13] =	fonts =			new ModuleFonts(true);

	//Measurements
	Timer timer = Timer();
	startupTime = Timer();
	frameTime = PerfTimer();
	lastSecFrameTime = PerfTimer();

	frames = 0;

}

Application::~Application()
{
	for (int i = 0; i < NUM_MODULES; ++i)
	{
		//Important: when deleting a pointer, set it to nullptr afterwards
		//It allows us for null check in other parts of the code
		delete modules[i];
		modules[i] = nullptr;
	}
}

bool Application::Awake()
{
	//maxFrameDuration = configFile.child("config").child("app").child("maxFrameDuration").attribute("value").as_int();
	// L1: DONE 3: Measure the amount of ms that takes to execute the Awake and LOG the result
	Timer timer = Timer();

	bool ret = LoadConfig();

	if (ret == true)
	{
		// L04: DONE 3: Read the title from the config file and set the windows title 
		// substitute "Video Game Template" string from the value of the title in the config file
		// also read maxFrameDuration 
		gameTitle.Create(configFile.child("config").child("app").child("title").child_value());
		/*window->SetTitle(gameTitle.GetString());*/
		maxFrameDuration = configFile.child("config").child("app").child("maxFrameDuration").attribute("value").as_int();

		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			// L04: DONE 4: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml

			ret = item->data->Awake(configFile.child("config").child(item->data->name.GetString()));
			item = item->next;
		}
	}

	LOG("Timer App Awake(): %f", timer.ReadMSec());

	return ret;

	
}

bool Application::Init()
{
	bool ret = true;

	// All modules (active and disabled) will be initialized
	for (int i = 0; i < NUM_MODULES && ret; ++i)
		ret = modules[i]->Init();

	// Only active modules will be 'started'
	for (int i = 0; i < NUM_MODULES && ret; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->Start() : true;



	return ret;
}

Update_Status Application::Update()
{

	Update_Status ret = Update_Status::UPDATE_CONTINUE;



	for (int i = 0; i < NUM_MODULES && ret == Update_Status::UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->PreUpdate() : Update_Status::UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == Update_Status::UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->Update() : Update_Status::UPDATE_CONTINUE;

	for (int i = 0; i < NUM_MODULES && ret == Update_Status::UPDATE_CONTINUE; ++i)
		ret = modules[i]->IsEnabled() ? modules[i]->PostUpdate() : Update_Status::UPDATE_CONTINUE;


	// L02: DONE 1: Cap the framerate of the gameloop
	// L02: DONE 2: Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected

	double currentDt = frameTime.ReadMs();
	if (maxFrameDuration > 0 && currentDt < maxFrameDuration) {
		uint32 delay = (uint32)(maxFrameDuration - currentDt);

		PerfTimer delayTimer = PerfTimer();
		SDL_Delay(delay);
		//LOG("We waited for %I32u ms and got back in %f ms",delay,delayTimer.ReadMs());
	}


	// L1: DONE 4: Calculate:
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
	static char title[256];
	sprintf_s(title, 256, "%s: Av.FPS: %.2f Last sec frames: %i Last dt: %.3f Time since startup: %I32u Frame Count: %I64u ",
		gameTitle.GetString(), averageFps, framesPerSecond, dt, secondsSinceStartup, frameCount);

	App->window->SetTitle(title);
	//window = SDL_CreateWindow("Ejercicio 4, Av.FPS: %.2f Last sec frames: %i Last dt: %.3f Time since startup: %I32u Frame Count: %I64u ", averageFps, framesPerSecond, dt, secondsSinceStartup, frameCount , SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE, flags);
	

	return ret;
}
 
bool Application::CleanUp()
{
	bool ret = true;

	for (int i = NUM_MODULES - 1; i >= 0 && ret; --i)
		ret = modules[i]->IsEnabled() ? modules[i]->CleanUp() : true;

	return ret;
}


// ---------------------------------------
const char* Application::GetTitle() const
{
	return gameTitle.GetString();
}
