#pragma once

#include "Globals.h"
#include <box2d/b2_math.h>
#include "Entity.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "Map.h"
#include "ModuleTextures.h"
#include "Module.h"



class Application;
class PhysBody;

enum class Direction {
	UP, RIGHT, DOWN, LEFT, DISABLE
};

class Enemy : public Entity
{
public:
	Enemy(Application* parent);

	virtual ~Enemy(){};

	update_status Update() override;

	void Render();

	void SetParameters(ModulePhysics* physics, Texture2D txt, float rot);

	bool TurnRight(float initialAngle, float finalAngle);

	float speed;
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void TurnBody(bool isGoingForward, bool isGoingRight, float torque, float speed) const;
	void OnCollisionEnd(PhysBody* bodyA, PhysBody* bodyB);

	void CheckSensor(PhysBody* sensor, bool collisionEnd);
	void CheckFinishLine();

	
public:
	bool finishedLap;

private:
	float currentSpeed = 0.f;

	bool isSpinningRight = false;
	bool isSpinning = false;

	bool forward = false;
	bool stopped = true;
	Direction dc;
};