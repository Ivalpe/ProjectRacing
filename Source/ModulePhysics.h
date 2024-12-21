#pragma once

#include "Module.h"
#include "Globals.h"

#include "box2d\box2d.h"

enum ColliderType {
	CAR,
	WALL,
	

};

// Module --------------------------------------
class PhysBody
{
public:
	PhysBody() : body(NULL)
	{}

	void GetPhysicPosition(int& x, int& y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;
public:
	int width, height;
	b2Body* body;
	Module* listenerptr = nullptr;
	ColliderType ctype;
};

class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	PhysBody* CreateRectangle(int x, int y, int width, int height);

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	

private:
	bool debug;
	b2World* world;
	b2Body* ground;
	
};