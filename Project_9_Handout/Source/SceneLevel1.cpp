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
	idlePortalAnim.PushBack({ 0, 0, 64, 64 });
	idlePortalAnim.PushBack({ 64, 0, 64, 64 });
	idlePortalAnim.PushBack({ 128, 0, 64, 64 });
	idlePortalAnim.PushBack({ 192, 0, 64, 64 });
	idlePortalAnim.PushBack({ 256, 0, 64, 64 });
	idlePortalAnim.PushBack({ 320, 0, 64, 64 });
	idlePortalAnim.PushBack({ 384, 0, 64, 64 });
	idlePortalAnim.PushBack({ 448, 0, 64, 64 });
	idlePortalAnim.loop = true;
	idlePortalAnim.speed = 0.2f;
}

SceneLevel1::~SceneLevel1()
{

}

// Load assets
bool SceneLevel1::Start()
{
	LOG("Loading background assets");

	bool ret = true;

	bgTexture = App->textures->Load("Assets/Sprites/fondito.png");
	grassTexture = App->textures->Load("Assets/Sprites/suelito.png");
	plantTexture = App->textures->Load("Assets/Sprites/plantitas.png");
	greenPortalTexture = App->textures->Load("Assets/Sprites/green_portal.png");
	purplePortalTexture = App->textures->Load("Assets/Sprites/purple_portal.png");
	App->audio->PlayMusic("Assets/Music/stage1.ogg", 1.0f);

	// Add colliders
	App->collisions->AddCollider({ 0, 221, 3930, 16 }, Collider::Type::WALL);
	App->collisions->AddCollider({ 31, 160, 2, 64 }, Collider::Type::PORTAL);
	currentPortalAnimation = &idlePortalAnim;

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
	currentPortalAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

// Update: draw background
Update_Status SceneLevel1::PostUpdate()
{
	// Draw everything --------------------------------------
	App->render->Blit(bgTexture, 0, 0, NULL);
	App->render->Blit(grassTexture, 0, 221, NULL);
	App->render->Blit(plantTexture, 280, 221, NULL);

	SDL_Rect greenPortalRect = currentPortalAnimation->GetCurrentFrame();
	App->render->Blit(greenPortalTexture, 0, 160, &greenPortalRect);
	SDL_Rect purplePortalRect = currentPortalAnimation->GetCurrentFrame();
	App->render->Blit(purplePortalTexture, 275, 60, &purplePortalRect);

	return Update_Status::UPDATE_CONTINUE;
}

bool SceneLevel1::CleanUp()
{
	App->player->Disable();
	App->enemies->Disable();

	return true;
}