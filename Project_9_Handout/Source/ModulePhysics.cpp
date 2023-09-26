#include "ModulePhysics.h"

#include "Application.h"
#include "ModulePlayer.h"

ModulePhysics::ModulePhysics(bool startEnabled) : Module(startEnabled)
{

}

ModulePhysics::~ModulePhysics()
{

}

bool ModulePhysics::Init()
{
	return true;
}

bool ModulePhysics::CleanUp()
{
	return true;
}

void ModulePhysics::Move(fPoint position, float force, float acceleration, float mass) {

	App->player->position.x += velocity * dt;
	velocity += ((force / mass) * dt);

}
