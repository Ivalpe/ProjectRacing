#pragma once

#include "Globals.h"
#include <box2d/b2_math.h>
#include "Module.h"


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

	virtual ~Entity() {
		for (auto s : sensors) delete s;
		sensors.clear();
	};

	int x, y;
	float rot;
	Texture2D texture;
	PhysBody* body = nullptr;

	PhysBody* GetBody() {
		return body;
	}

	void GetPosition(int& x, int& y) const;

	float GetBodyAngle() const;

	float TurnBody(bool isGoingForward, bool isGoingRight, float torque, float speed) const;

	void SetPosition(Vector2 pos);

	Texture2D GetTexture() {
		return texture;
	}

	update_status Update() = 0;

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {

	};

	virtual void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {

	};
  
	float MaxSpeed = 5.f;
	float MaxAngle = 3.14f;
	float MinSpeed = 0.75f;
	float forceIncrement = 10.f;
	float torqueSpeed = 0.8f * SCALE;
	int Lap = 1;

	std::vector<CheckpointSensor*> sensors;

	void CheckSensor(PhysBody* sensor, bool collisionEnd);
	void CheckFinishLine();

};