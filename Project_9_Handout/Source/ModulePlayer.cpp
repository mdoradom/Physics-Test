#include "ModulePlayer.h"

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"

#include <stdio.h>
#include "ModulePhysics.h"

#include "Globals.h"

#include <string>

ModulePlayer::ModulePlayer(bool startEnabled) : Module(startEnabled)
{
	// idle animation - just one sprite
	idleAnim.PushBack({ 0, 256, 64, 64 });
	idleAnim.PushBack({ 64, 256, 64, 64 });
	idleAnim.PushBack({ 128, 256, 64, 64 });
	idleAnim.PushBack({ 0, 320, 64, 64 });
	idleAnim.PushBack({ 64, 320, 64, 64 });
	idleAnim.PushBack({ 128, 320, 64, 64 });
	idleAnim.PushBack({ 0, 384, 64, 64 });
	idleAnim.PushBack({ 64, 384, 64, 64 });
	idleAnim.PushBack({ 128, 382, 64, 64 });
	idleAnim.PushBack({ 0, 448, 64, 64 });
	idleAnim.PushBack({ 64, 448, 64, 64 });
	idleAnim.PushBack({ 128, 448, 64, 64 });
	idleAnim.loop = true;
	idleAnim.speed = 0.08f;

	// move right
	walkRigtAnim.PushBack({ 0, 192, 64, 64 });
	walkRigtAnim.PushBack({ 64, 192, 64, 64 });
	walkRigtAnim.PushBack({ 128, 192, 64, 64 });
	walkRigtAnim.PushBack({ 192, 192, 64, 64 });
	walkRigtAnim.loop = true;
	walkRigtAnim.speed = 0.1f;

	// Move left
	walkLeftAnim.PushBack({ 0, 128, 64, 64 });
	walkLeftAnim.PushBack({ 64, 128, 64, 64 });
	walkLeftAnim.PushBack({ 128, 128, 64, 64 });
	walkLeftAnim.PushBack({ 192, 128, 64, 64 });
	walkLeftAnim.loop = true;
	walkLeftAnim.speed = 0.1f;
}

ModulePlayer::~ModulePlayer()
{

}

bool ModulePlayer::Start()
{
	LOG("Loading player textures");

	bool ret = true;

	texture = App->textures->Load("Assets/Sprites/gatita.pngg");
	currentAnimation = &idleAnim;
	

	laserFx = App->audio->LoadFx("Assets/Fx/laser.wav");
	explosionFx = App->audio->LoadFx("Assets/Fx/explosion.wav");

	position.x = 150;
	position.y = 162;

	destroyed = false;

	collider = App->collisions->AddCollider({ (int)position.x, (int)position.y, 64, 64 }, Collider::Type::PLAYER, this);

	return ret;
}

Update_Status ModulePlayer::Update()
{
	// Moving the player with the camera scroll
	// App->player->position.x += 1;
	

	if (App->input->keys[SDL_SCANCODE_A] == Key_State::KEY_REPEAT)
	{
		launchSpeed--;
	}

	if (App->input->keys[SDL_SCANCODE_D] == Key_State::KEY_REPEAT)
	{
		launchSpeed++;
	}

	if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT)
	{
		launchAngle--;
	}

	if (App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT)
	{
		launchAngle++;
	}

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{
		App->physics->ParabolicShot(App->physics->balls[0], launchAngle, launchSpeed);
	}

	if (App->input->keys[SDL_SCANCODE_F1] == Key_State::KEY_DOWN) {
		debug = !debug;
	}

	App->render->DrawLineWithAngleAndSpeed(METERS_TO_PIXELS(App->physics->balls[0].x), SCREEN_HEIGHT - METERS_TO_PIXELS(App->physics->balls[0].y), launchAngle, launchSpeed , 255, 0, 0, 255);

	collider->SetPos((float)position.x, (float)position.y);

	currentAnimation->Update();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModulePlayer::PostUpdate()
{
	if (!destroyed)
	{
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		App->render->Blit(texture, position.x, position.y, &rect);
	}

	if (debug) {

		std::string deltaTime = "deltaTime: " + std::to_string((float)App->physics->dt) + " ms";
		std::string angleString = "launchAngle: " + std::to_string((int)launchAngle) + " degrees";
		std::string initialSpeedString = "launchSpeed: " + std::to_string((int)App->player->launchSpeed) + " m/s";
		PhysBall* lastBall = &App->physics->balls.back();
		std::string lastBallInfo = "Ball Info = "
			"Pos: (" + std::to_string((int)lastBall->x) + ", " + std::to_string((int)lastBall->y) + ")"
			" | Vel: (" + std::to_string((int)lastBall->vx) + ", " + std::to_string((int)lastBall->vy) + ")"
			" | Accel: (" + std::to_string((float)lastBall->ax) + ", " + std::to_string((float)lastBall->ay) + ")";
		
		App->fonts->drawText(deltaTime.c_str(), { 255,255,255 }, 20, 50);
		App->fonts->drawText(angleString.c_str(), { 255,255,255 }, 20, 75);
		App->fonts->drawText(initialSpeedString.c_str(), { 255,255,255 }, 20, 100);
		App->fonts->drawText(lastBallInfo.c_str(), { 255,255,255 }, 20, 125);

	}

	return Update_Status::UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{

}