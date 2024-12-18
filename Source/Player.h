#pragma once

#include "Globals.h"
#include <box2d/b2_math.h>
#include "Entity.h"
#include "ModulePhysics.h"

class Application;
class PhysBody;

class Player : public Entity
{
public:
	Player();

	virtual ~Player(){};

	void Update() override;

	void SetParameters(ModulePhysics* physics, Texture2D txt);

	float speed;
private:
};