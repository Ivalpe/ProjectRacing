#pragma once

#include "Globals.h"
#include <box2d/b2_math.h>

class Application;
class PhysBody;

class Entity
{
public:
	Entity() {};

	virtual ~Entity() {};

	int x, y;
	float rot;
	Texture2D texture;
	PhysBody* body = nullptr;

	PhysBody* GetBody() {
		return body;
	}

	void GetPosition(int& x, int& y) const;

	Texture2D GetTexture() {
		return texture;
	}

	virtual void Update() = 0;

	float MaxSpeed = 10;
};