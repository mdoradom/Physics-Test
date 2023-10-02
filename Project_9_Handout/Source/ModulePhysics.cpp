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

	velocity.x = 35;
	velocity.y = 25;

	return true;
}

bool ModulePhysics::CleanUp()
{
	return true;
}

void ModulePhysics::Move(fPoint position, float force, float acceleration, float mass) {

	// TODO cambiar esto a la posicion que pasamos por parámetro, pero hay que pasarla como referencia
	App->player->position.x += velocity.x * dt;
	velocity.x += ((force / mass) * dt);

}

void ModulePhysics::ParabolicShot(fPoint position, float mass, float angle) {

	// EJE X
	App->player->position.x += (velocity.x * sin(angle)) * dt;

	// EJE Y
	App->player->position.y += (velocity.y * cos(angle)) * dt - 0.5 * GRAVITY * pow(dt, 2);
	velocity.y += GRAVITY * dt;

}
