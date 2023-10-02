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

	texture = App->textures->Load("Assets/Sprites/gatita.png");
	currentAnimation = &idleAnim;

	laserFx = App->audio->LoadFx("Assets/Fx/laser.wav");
	explosionFx = App->audio->LoadFx("Assets/Fx/explosion.wav");

	position.x = 150;
	position.y = 162;

	destroyed = false;

	collider = App->collisions->AddCollider({ (int)position.x, (int)position.y, 64, 64 }, Collider::Type::PLAYER, this);

	// TODO 0: Notice how a font is loaded and the meaning of all its arguments 
	char lookupTable[] = { "!  ,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz" };
	scoreFont = App->fonts->Load("Assets/Fonts/rtype_font.png", "! @,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz", 1);

	// TODO 4: Try loading "rtype_font3.png" that has two rows to test if all calculations are correct

	return ret;
}

Update_Status ModulePlayer::Update()
{
	// Moving the player with the camera scroll
	// App->player->position.x += 1;

	/*
	if (speed > 0) {
		App->physics->Move(position, 0, speed - 10, mass);
	}
	*/

	App->physics->ParabolicShot(position, mass, 90.0);
	

	if (App->input->keys[SDL_SCANCODE_A] == Key_State::KEY_REPEAT)
	{
		// position.x -= speed;

		if (speed > 2) {
			speed--;
		}
		if (currentAnimation != &walkLeftAnim)
		{
			walkLeftAnim.Reset();
			currentAnimation = &walkLeftAnim;
		}

		App->physics->Move(position, -10000, speed, mass);
		
	}

	if (App->input->keys[SDL_SCANCODE_D] == Key_State::KEY_REPEAT)
	{
		
		if (speed < 2) {
			speed++;
		}
		if (currentAnimation != &walkRigtAnim)
		{
			walkRigtAnim.Reset();
			currentAnimation = &walkRigtAnim;
		}
		
		App->physics->Move(position, 10000, speed, mass);

	}

	/*

	if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_REPEAT)
	{
		position.y += speed;
		if (currentAnimation != &downAnim)
		{
			downAnim.Reset();
			currentAnimation = &downAnim;
		}
	}

	if (App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_REPEAT)
	{
		position.y -= speed;
		if (currentAnimation != &upAnim)
		{
			upAnim.Reset();
			currentAnimation = &upAnim;
		}
	}

	*/

	if (App->input->keys[SDL_SCANCODE_SPACE] == Key_State::KEY_DOWN)
	{

		/*
		Particle* newParticle = App->particles->AddParticle(App->particles->laser, position.x + 20, position.y, Collider::Type::PLAYER_SHOT);
		newParticle->collider->AddListener(this);
		App->audio->PlayFx(laserFx);
		*/
	}

	
	/*

	// If no up/down movement detected, set the current animation back to idle
	if (App->input->keys[SDL_SCANCODE_S] == Key_State::KEY_IDLE
		&& App->input->keys[SDL_SCANCODE_W] == Key_State::KEY_IDLE)
		currentAnimation = &idleAnim;

	*/

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

	// Draw UI (score) --------------------------------------
	sprintf_s(scoreText, 10, "%7d", score);

	// TODO 3: Blit the text of the score at the bottom of the screen

	App->fonts->BlitText((SCREEN_WIDTH/2), 10, scoreFont, scoreText);

	return Update_Status::UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
{
	//if (c1 == collider && destroyed == false)
	//{
	//	App->particles->AddParticle(App->particles->explosion, position.x, position.y, Collider::Type::NONE, 9);
	//	App->particles->AddParticle(App->particles->explosion, position.x + 8, position.y + 11, Collider::Type::NONE, 14);
	//	App->particles->AddParticle(App->particles->explosion, position.x - 7, position.y + 12, Collider::Type::NONE, 40);
	//	App->particles->AddParticle(App->particles->explosion, position.x + 5, position.y - 5, Collider::Type::NONE, 28);
	//	App->particles->AddParticle(App->particles->explosion, position.x - 4, position.y - 4, Collider::Type::NONE, 21);

	//	App->audio->PlayFx(explosionFx);
	//	App->fade->FadeToBlack((Module*)App->sceneLevel_1, (Module*)App->sceneIntro, 60);

	//	destroyed = true;
	//}

	//if (c1->type == Collider::Type::PLAYER_SHOT && c2->type == Collider::Type::ENEMY)
	//{
	//	score += 23;
	//}
}