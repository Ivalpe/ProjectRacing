#pragma once

#include "Globals.h"
#include <box2d/b2_math.h>
#include "Entity.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Module.h"
#include "Timer.h"

enum ItemType {
	TURBO,
	COIN,
	SHELL
};


class Application;
class PhysBody;

class Item : public Entity
{
public:
	Item(Application* parent);

	virtual ~Item() {};

	void SetParameters(ModulePhysics* physics, int texIndex); //start

	update_status Update() override;

	void Render(); //postupdate

	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

public:
	bool isPicked;
private:
	std::vector<Texture2D>itemTex;
	Texture2D boxTex;
	ItemType type;
	int texIndex;
	int width, height;
	Timer reappearTimer;
	float reappearTime = 5.0f;
};