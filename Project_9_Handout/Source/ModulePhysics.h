#ifndef __MODULE_PHYSICS_H__
#define __MODULE_PHYSICS_H__

#include "Module.h"
#include "p2Point.h"
#include <vector>
#include <SDL/include/SDL.h>
#include "Application.h"
#include "Globals.h"

using namespace std;

class PhysBall
{
public:
	// Position
	// You could also use an array/vector
	float x;
	float y;

	// Velocity
	float vx;
	float vy;

	// Acceleration
	float ax;
	float ay;

	// Force (total) applied to the ball
	float fx;
	float fy;

	// Mass
	float mass;

	// Aerodynamics stuff
	float surface; // Effective wet surface
	float cl; // Aerodynamic Lift coefficient
	float cd; // Aerodynamic Drag coefficient
	float b; // Hydrodynamic Drag coefficient

	// Coefficients of friction & restitution (for bounces)
	float coef_friction;
	float coef_restitution;

	// Shape
	float radius;

	// Has physics enabled?
	bool physics_enabled = true;
};

// Class: Ground
class Ground : public SDL_Rect {
	public:
		float x, y, w, h; // Geometry (x,y,w,h)
		SDL_Rect pixels(); // Convert geometry to pixels to draw w/ SDL
};

// Class: Square
class Square : public Ground {
};

// Class: Water
class Water : public Ground
{
public:
	float density; // Density of fluid
	float vx; // Velocity x
	float vy; // Velocity y
};

// Class: Atmosphere
class Atmosphere
{
public:
	float density; // Density of air
	float windx; // Wind x
	float windy; // Wind y
};

class ModulePhysics : public Module
{
public:
    // Constructor
    ModulePhysics(bool startEnabled);

    // Destructor
    ~ModulePhysics();

    bool Init() override;
	bool Start() override;
	Update_Status PreUpdate() override;
	Update_Status PostUpdate() override;
    bool CleanUp() override;

    void Move();
    void ParabolicShot(PhysBall& ball, float angle, float initialSpeed);

    // Physics objects
    std::vector<PhysBall> balls;
    Atmosphere atmosphere;
    Ground ground;
    Water water;
	Square topSquare, leftSquare, rightSquare, downSquare;

    // Misc
    const float GRAVITY = -9.81;
    float dt = 1.0 / 60.0;

private:
    // Compute modulus of a vector
    float modulus(float vx, float vy);

	// Detect collision between circle and rectangle
	bool check_collision_circle_rectangle(float cx, float cy, float cr, float rx, float ry, float rw, float rh);

    // Compute Aerodynamic Drag force
    void compute_aerodynamic_drag(float& fx, float& fy, const PhysBall& ball, const Atmosphere& atmosphere);

    // Compute Hydrodynamic Drag force
    void compute_hydrodynamic_drag(float& fx, float& fy, const PhysBall& ball, const Water& water);

    // Compute Hydrodynamic Buoyancy force
    void compute_hydrodynamic_buoyancy(float& fx, float& fy, const PhysBall& ball, const Water& water);

    // Integration scheme: Velocity Verlet
    void integrator_velocity_verlet(PhysBall& ball, float dt);

    // Detect collision with ground
    bool is_colliding_with_ground(const PhysBall& ball, const Ground& ground);

	// Detect collision with square
    bool is_colliding_with_square(const PhysBall& ball, const Square& square);

    // Detect collision with water
    bool is_colliding_with_water(const PhysBall& ball, const Water& water);
};

#endif // __MODULE_PHYSICS_H__

// Implement the functions in the ModulePhysics.cpp file
