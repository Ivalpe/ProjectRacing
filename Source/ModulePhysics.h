#pragma once

#include "Module.h"
#include "Globals.h"
#include "Entity.h"
#include "box2d\box2d.h"

enum ColliderType {
	CAR,
	WALL,
	SENSOR,
	ITEM,
	FINISH_LINE,
	UP,
	RIGHT,
	DOWN,
	LEFT

};

//enum bodyType {
//	DYNAMIC,
//	STATIC,
//	KINEMATIC
//};

// Module --------------------------------------
class PhysBody
{
public:
	PhysBody() : listenerptr(NULL), body(NULL)
	{}

	~PhysBody()
	{
		delete listenerptr;
	}

	void GetPhysicPosition(int& x, int& y) const;
	float GetRotation() const;
	float GetAngleRotation() const;

	float ScalarLinearVelocity() const;

	void ResetLinearVelocity() const;
	void ResetAngularVelocity() const;
	void ApplyMovingForce(float speed) const;
	void TurnWithTorque(float torque) const;

	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;
public:
	int id = -1;
	int width, height;
	b2Body* body;
	Entity* listenerptr = nullptr;
	ColliderType ctype;
};

class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	PhysBody* CreateRectangle(int x, int y, int width, int height, b2BodyType bodyType);
	PhysBody* CreateCircle(int x, int y, int radious, b2BodyType type);
	PhysBody* CreateChain(int x, int y, int* points, int size, b2BodyType type);

	
	PhysBody* CreateCircleSensor(int x, int y, int radious, b2BodyType type);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, b2BodyType type);
	int RayCastGlobal(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y, b2Body* body) const;



	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	bool GetDebug() {
		return debug;
	}

	void DeleteBody(b2Body* body) {
		world->DestroyBody(body);
	}

private:
	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
	std::vector<b2Body*> bodies;
};