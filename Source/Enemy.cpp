
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
	float rot = -90 * PI / 180.0f;

	body->body->SetTransform({ PIXEL_TO_METERS(x), PIXEL_TO_METERS(y) }, rot);
	body->listenerptr = this;
}

update_status Enemy::Update() {

	update_status ret = UPDATE_CONTINUE;

	currentSpeed = body->ScalarLinearVelocity();

	speed = 0;
	if ((int)currentSpeed * 10000 == 0) {
		if (currentSpeed <= MaxSpeed) speed -= forceIncrement * 2;
	}
	else if (currentSpeed <= MaxSpeed) speed -= forceIncrement;

	float rayLength = 50.0f;

	float x2 = x + rayLength * cos(body->GetRotation());
	float y2 = y - cos(body->GetRotation()) + rayLength * sin(body->GetRotation());

	float x3 = x - rayLength * cos(body->GetRotation());
	float y3 = y - cos(body->GetRotation()) - rayLength * sin(body->GetRotation());


	float normalX, normalY;
	int test = body->RayCast(x, y, x2, y2, normalX, normalY);
	int test2 = body->RayCast(x, y, x3, y3, normalX, normalY);

	if (App->physics->GetDebug()) {
		App->renderer->DrawRectangleDebug(x3, y3, 10, 10, RED);
		App->renderer->DrawRectangleDebug(x2, y2, 10, 10, YELLOW);
	}

	static b2Vec2 velocity = b2Vec2(0, 0);
	if (abs(currentSpeed) >= MinSpeed) {
		isSpinning = false;
		if (test != -1) {
			if (GetBodyAngle() < MaxAngle) {
				if (isSpinningRight) TurnBody(forward, isSpinningRight, torqueSpeed, currentSpeed);
				isSpinningRight = true;
				isSpinning = true;
				TurnBody(forward, isSpinningRight, torqueSpeed, currentSpeed);
			}
		}
		else if (test2 != -1) {
			if (GetBodyAngle() > -MaxAngle) {
				if (!isSpinningRight) TurnBody(forward, isSpinningRight, torqueSpeed, currentSpeed);
				isSpinningRight = false;
				isSpinning = true;
				TurnBody(forward, isSpinningRight, torqueSpeed, currentSpeed);
			}
		}

		if (!isSpinning) body->ResetAngularVelocity();

	}
	else body->ResetAngularVelocity();


	//body->ApplyMovingForce(speed);


	GetPosition(x, y);

	Rectangle source = { 0.0f , 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { x, y, (float)texture.width * SCALE , (float)texture.height * SCALE };
	Vector2 origin = { ((float)texture.width / (2.0f)) * SCALE, ((float)texture.height / (2.0f)) * SCALE };
	float rotation = body->GetRotation() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);

	return ret;
}