#pragma once

#include "Globals.h"
#include <box2d/b2_math.h>

class Application;
class PhysBody;

class Entity
{
public:
	Entity(){};

	virtual ~Entity(){};

	b2Vec2 position;
	Texture2D texture;
	PhysBody* body;

	PhysBody* getBody() {
		return body;
	}

	Texture2D getTexture() {
		return texture;
	}
};