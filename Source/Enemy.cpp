
#include "Globals.h"
#include "Entity.h"
#include "Module.h"
#include "Enemy.h"
#include "box2d/b2_math.h"
#include <algorithm>

Enemy::Enemy(Application* parent) : Entity(parent)
{
	speed = 0.f;
}

void Enemy::SetParameters(ModulePhysics* physics, Texture2D txt) {
	texture = txt;
	body = physics->CreateRectangle(0, 0, SPRITE_WIDTH * SCALE, SPRITE_HEIGHT * SCALE, b2_dynamicBody);
	x = 100;
	y = 100;

	body->body->SetTransform({ PIXEL_TO_METERS(x), PIXEL_TO_METERS(y) }, body->body->GetTransform().q.GetAngle());
	body->listenerptr = this;
}

update_status Enemy::Update() {

	update_status ret = UPDATE_CONTINUE;

	GetPosition(x, y);

	Rectangle source = { 0.0f , 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { x + App->renderer->camera.x , y + App->renderer->camera.y, (float)texture.width * SCALE , (float)texture.height * SCALE };
	Vector2 origin = { ((float)texture.width / (2.0f)) * SCALE, ((float)texture.height / (2.0f)) * SCALE };
	float rotation = body->GetRotation() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);

	return ret;
}