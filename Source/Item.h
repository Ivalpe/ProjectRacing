#pragma once

#include "Globals.h"
#include <box2d/b2_math.h>
#include "Entity.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Module.h"



class Application;
class PhysBody;

class Item : public Entity
{
public:
	Item(Application* parent);

	virtual ~Item() {};

	void SetParameters(ModulePhysics* physics, Texture2D itemTex); //start

	update_status Update() override;

	void Render(); //postupdate

	/*void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);*/

public:
	bool isPicked;
private:
	Texture2D boxTex, itemTex;
};