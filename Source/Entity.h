#pragma once

#include "Globals.h"
#include <box2d/b2_math.h>
#include "Module.h"


class Application;
class PhysBody;

class Entity : public Module
{
public:
	Entity(Application* parent);

	virtual ~Entity() {};

	int x, y;
	float rot;
	Texture2D texture;
	PhysBody* body = nullptr;

	PhysBody* GetBody() {
		return body;
	}

	void GetPosition(int& x, int& y) const;
	void SetPosition(Vector2 pos);

	Texture2D GetTexture() {
		return texture;
	}

	update_status Update() = 0;

	float MaxSpeed = 10;
};