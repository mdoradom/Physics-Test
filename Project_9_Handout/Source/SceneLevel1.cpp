#include "SceneLevel1.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleWindow.h"
#include "Globals.h"

SceneLevel1::SceneLevel1(bool startEnabled) : Module(startEnabled)
{
}

SceneLevel1::~SceneLevel1()
{

}

// Load assets
bool SceneLevel1::Start()
{
	SDL_Renderer* rendererSquare = NULL;

	App->physics->Enable();

	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/fondito.png");
	grassTexture = App->textures->Load("Assets/Sprites/suelito.png");
	plantTexture = App->textures->Load("Assets/Sprites/plantitas.png");
	//App->audio->PlayMusic("Assets/Music/stage1.ogg", 1.0f);

	// Add colliders
	//App->collisions->AddCollider({ 0, 221, 3930, 16 }, Collider::Type::WALL);

	// Remder camear
	App->render->camera.x = 0;
	App->render->camera.y = 0;

	App->player->Enable();
	App->enemies->Enable();

	// ---------------------

	LOG("Creating Physics 2D environment");

	return ret;
}

Update_Status SceneLevel1::Update()
{
	// if wanted to move the camera
	//App->render->camera.x += 3;

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneLevel1::PostUpdate()
{
	// Draw everything --------------------------------------
	/*
	App->render->Blit(bgTexture, 0, 0, NULL);
	App->render->Blit(grassTexture, 0, 221, NULL);
	App->render->Blit(plantTexture, 280, 221, NULL);
	*/
	

	return Update_Status::UPDATE_CONTINUE;
}

bool SceneLevel1::CleanUp()
{
	App->player->Disable();
	App->enemies->Disable();

	return true;
}