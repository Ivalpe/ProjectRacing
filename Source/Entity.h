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
	void SetPosition(Vector2 pos);

	Texture2D GetTexture() {
		return texture;
	}

	void SetCamera(Rectangle cam) {
		camera = cam;
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {

	};

	virtual void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {

	};

	virtual void Update() = 0;

	float MaxSpeed = 5;
	Rectangle camera;
};