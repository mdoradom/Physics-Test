#ifndef __MODULE_PHYSICS_H__
#define __MODULE_PHYSICS_H__

#include "Module.h"
#include "p2Point.h"

class ModulePhysics: public Module
{
public:
	//Constructor
	ModulePhysics(bool startEnabled);

	//Destructor
	~ModulePhysics();

	bool Init() override;

	bool CleanUp() override;

	void Move(fPoint position, float force, float acceleration, float mass);

	//void Collide() override;

public:
	const float GRAVITY = -9.81;
	float dt = 1.0 / 60; // TODO adaptar a refresco de pantalla
	float velocity = 0;
};

#endif // __ModulePhysics_H__