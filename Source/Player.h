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

	void SetParameters(ModulePhysics* physics, Texture2D txt) {
		texture = txt;
		body = physics->CreateRectangle(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
		position.Set(0, 0);
	}

	void SetPosition(b2Vec2 pos) {
		position.Set(pos.x, pos.y);
	}
};