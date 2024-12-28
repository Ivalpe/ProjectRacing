#include "Item.h"
#include "ModulePhysics.h"
#include "Globals.h"
#include "Entity.h"

Item::Item(Application* parent) : Entity(parent)
{
	
}


void Item::SetParameters(ModulePhysics* physics, Texture2D itemTex) {

	//initilize textures
	texture = LoadTexture("Assets/Textures/itemBox.png");

	// L08 TODO 4: Add a physics to an item - initialize the physics body
	
	body = physics->CreateRectangle(0, 0, SPRITE_HEIGHT * SCALE, SPRITE_HEIGHT * SCALE, b2_dynamicBody);
	// L08 TODO 7: Assign collider type
	body->ctype = ColliderType::ITEM;

	
}

update_status Item::Update()
{
	update_status ret = UPDATE_CONTINUE;
	// L08 TODO 4: Add a physics to an item - update the position of the object from the physics.  

	

	return ret;
}

void Item::Render() {
	GetPosition(x, y);
	Rectangle source = { 0.0f , 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { x + App->renderer->camera.x, y + App->renderer->camera.y, (float)texture.width * SCALE , (float)texture.height * SCALE };
	Vector2 origin = { ((float)texture.width / (2.0f)) * SCALE, ((float)texture.height / (2.0f)) * SCALE };
	float rotation = body->GetRotation() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
}