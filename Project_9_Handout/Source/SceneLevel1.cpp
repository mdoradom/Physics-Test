#include "SceneLevel1.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleEnemies.h"
#include "ModulePlayer.h"

SceneLevel1::SceneLevel1(bool startEnabled) : Module(startEnabled)
{
	// idle animation - just one sprite
	plantAnim.PushBack({ 0, 0, 32, 32 });
	plantAnim.PushBack({ 32, 0, 32, 32 });
	plantAnim.PushBack({ 0, 32, 32, 32 });
	plantAnim.loop = true;
	plantAnim.speed = 0.07f;
}

SceneLevel1::~SceneLevel1()
{

}

// Load assets
bool SceneLevel1::Start()
{
	LOG("Loading background assets");

	bool ret = true;
	staying = false;

	bgTexture = App->textures->Load("Assets/Sprites/fondito.png");
	grassTexture = App->textures->Load("Assets/Sprites/suelito.png");
	plantTexture = App->textures->Load("Assets/Sprites/plantitas.png");
	currentAnimation = &plantAnim;

	App->audio->PlayMusic("Assets/Music/stage1.ogg", 1.0f);

	// Add colliders
	App->collisions->AddCollider({ 0, 221, 3930, 16 }, Collider::Type::WALL);
	App->collisions->AddCollider({ 280, 189, 32, 32 }, Collider::Type::ENEMY);
	currentAnimation = &plantAnim;

	// Remder camear
	App->render->camera.x = 0;
	App->render->camera.y = 0;

	App->player->Enable();
	App->enemies->Enable();

	return ret;
}

Update_Status SceneLevel1::Update()
{
	// if wanted to move the camera
	//App->render->camera.x += 3;

	if (staying)
		currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneLevel1::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture, 0, 0, NULL);
	App->render->Blit(grassTexture, 0, 221, NULL);

	if (!App->player->destroyed)
	{
		App->render->Blit(App->player->texture, App->player->position.x, App->player->position.y, &App->player->rect);
	}

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	App->render->Blit(plantTexture, 280, 190, &rect);

	return Update_Status::UPDATE_CONTINUE;
}

bool SceneLevel1::CleanUp()
{
	App->player->Disable();
	App->enemies->Disable();

	return true;
}
