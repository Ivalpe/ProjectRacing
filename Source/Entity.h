#pragma once

#include "Globals.h"
#include <box2d/b2_math.h>
#include "Module.h"


enum EntityType {
	PLAYER,
	AI
};

class Application;
class PhysBody;

struct CheckpointSensor {
	int id;
	bool active;
	bool changeable;
};

class Entity : public Module
{
public:
	Entity(Application* parent);

	virtual ~Entity() {};

	int x, y;
	float rot;
	Texture2D texture;
	PhysBody* body = nullptr;

	EntityType carType;



	PhysBody* GetBody() {
		return body;
	}

	void GetPosition(int& x, int& y) const;

	float GetBodyAngle() const;

	void TurnBody(bool isGoingForward, bool isGoingRight, float torque, float speed) const;

	void SetPosition(Vector2 pos);

	Texture2D GetTexture() {
		return texture;
	}

	bool CleanUp() {
		delete body;
		return true;
	}

	update_status Update() = 0;

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {

	};

	void Render();

	virtual void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {

	};
  
	float MaxSpeed = 5.f;
	float MaxAngle = 3.14f;
	float MinSpeed = 0.75f;
	float forceIncrement = 10.f;
	float torqueSpeed = 0.8f * SCALE;
	int Lap = 1;
	int cpCount;

	std::vector<CheckpointSensor> sensors;


};