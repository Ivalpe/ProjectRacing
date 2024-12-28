#include "Item.h"
#include "ModulePhysics.h"
#include "Globals.h"
#include "Entity.h"




Item::Item(Application* parent) : Entity(parent)
{
	
}


void Item::SetParameters(ModulePhysics* physics, int _texIndex) {

	//initilize textures
	texture = LoadTexture("Assets/item box-outlined.png");

	
	itemTex.push_back(LoadTexture("Assets/mushroom power up.png"));
	itemTex.push_back(LoadTexture("Assets/coin power up.png"));
	itemTex.push_back(LoadTexture("Assets/shell power up.png"));

	// L08 TODO 4: Add a physics to an item - initialize the physics body
	
	body = physics->CreateRectangleSensor(0, 0, texture.width * SCALE, texture.height * SCALE, b2_staticBody);
	// L08 TODO 7: Assign collider type
	body->ctype = ColliderType::ITEM;
	body->listenerptr = this;
	isPicked = false;

	texIndex = _texIndex;

	switch (texIndex) {
	case 0:
		type = TURBO;
	case 1:
		type = COIN;
	case 2:
		type = SHELL;
	}
	
}

update_status Item::Update()
{
	update_status ret = UPDATE_CONTINUE;
	// L08 TODO 4: Add a physics to an item - update the position of the object from the physics.  
	
	if (isPicked) {
		if (reappearTimer.ReadSec() > reappearTime) {
			isPicked = false;
			body->body->SetEnabled(true);
		}
		else {
			body->body->SetEnabled(false);
		}
	}
	

	return ret;
}

void Item::Render() {
	GetPosition(x, y);
	Rectangle source = { 0.0f , 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { x + App->renderer->camera.x, y + App->renderer->camera.y, (float)texture.width * SCALE , (float)texture.height * SCALE };
	Vector2 origin = { ((float)texture.width / (2.0f)) * SCALE, ((float)texture.height / (2.0f)) * SCALE };
	float rotation = body->GetRotation() * RAD2DEG;

	if (!isPicked) {
		DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	}

	
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype) {
	case ColliderType::CAR:
		if (!isPicked) {
			isPicked = true;
			reappearTimer.Start();
		}
		break;
	default:
		break;
	}
	
}
void Item::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype) {
	case ColliderType::CAR:
		/*reappearTimer.Start();*/
		break;
	default:
		break;
	}

}