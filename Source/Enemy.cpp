
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

void Enemy::SetParameters(ModulePhysics* physics, Texture2D txt, float rot) {
	texture = txt;
	body = physics->CreateRectangle(0, 0, SPRITE_WIDTH * SCALE, SPRITE_HEIGHT * SCALE, b2_dynamicBody);

	carType = AI;

	body->body->SetTransform({ PIXEL_TO_METERS(x), PIXEL_TO_METERS(y) }, rot);
	body->listenerptr = this;
}

update_status Enemy::Update() {

	update_status ret = UPDATE_CONTINUE;

	static b2Vec2 velocity = b2Vec2(0, 0);
	currentSpeed = body->ScalarLinearVelocity();

	float rayLength = 200.0f;

	float x2 = x + rayLength * cos(body->GetRotation());
	float y2 = y - cos(body->GetRotation()) + rayLength * sin(body->GetRotation());

	float x3 = x - rayLength * cos(body->GetRotation());
	float y3 = y - cos(body->GetRotation()) - rayLength * sin(body->GetRotation());

	float normalX, normalY;
	int test = App->physics->RayCastGlobal(x, y, x3, y3, normalX, normalY, body->body);
	int test2 = App->physics->RayCastGlobal(x, y, x2, y2, normalX, normalY, body->body);
	//int test = App->map->RayCastGlobal(x, y, x2, y2, normalX, normalY);
	//int test2 = App->map->RayCastGlobal(x, y, x3, y3, normalX, normalY);


	if (App->physics->GetDebug()) {
		App->renderer->DrawRectangleDebug(x3, y3, 10, 10, RED);
		App->renderer->DrawRectangleDebug(x2, y2, 10, 10, YELLOW);
	}

	TraceLog(LOG_INFO, "%d", test2);

	if (abs(currentSpeed) >= MinSpeed) {
		isSpinning = false;
		if (test != -1) {
			isSpinning = true;
			TurnBody(forward, true, torqueSpeed * 5, currentSpeed);
		}
		else if (test2 != -1) {
			isSpinning = true;
			TurnBody(forward, false, torqueSpeed * 5, currentSpeed);
		}
		if (!isSpinning) body->ResetAngularVelocity();
	}
	else body->ResetAngularVelocity();

	speed = 0;
	if ((int)currentSpeed * 10000 == 0) {
		if (currentSpeed <= MaxSpeed) speed -= forceIncrement * 2;
	}
	else if (currentSpeed <= MaxSpeed) speed -= forceIncrement;

	body->ApplyMovingForce(speed);

	GetPosition(x, y);

	return ret;
}

void Enemy::Render() {
	GetPosition(x, y);
	Rectangle source = { 0.0f , 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { x, y, (float)texture.width * SCALE , (float)texture.height * SCALE };
	Vector2 origin = { ((float)texture.width / (2.0f)) * SCALE, ((float)texture.height / (2.0f)) * SCALE };
	float rotation = body->GetRotation() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype) {
	case ColliderType::WALL:
		TraceLog(LOG_INFO, "COLLISION");
		stopped = true;
		break;
	case ColliderType::SENSOR:
		CheckSensor(physB, false);
		break;
	case ColliderType::FINISH_LINE:
		CheckFinishLine();
		CheckSensor(physB, true);
		break;
	default:
		break;
	}
}

void Enemy::TurnBody(bool isGoingForward, bool isGoingRight, float torque, float speed) const {
	float FinalTorque = 0.f;
	if (isGoingRight) {
		if (isGoingForward) FinalTorque = torque / abs(speed);
		else FinalTorque = -torque / abs(speed);
	}
	else {
		if (isGoingForward) FinalTorque = -torque / abs(speed);
		else FinalTorque = torque / abs(speed);
	}

	body->TurnWithTorque(FinalTorque);
}

void Enemy::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype) {
	case ColliderType::WALL:
		break;
	case ColliderType::SENSOR:
		CheckSensor(physB, true);
		break;
	case ColliderType::FINISH_LINE:

		break;
	default:
		break;
	}
}

void Enemy::CheckSensor(PhysBody* sensor, bool collisionEnd) {
	int i = 0;
	while (i < sensors.size()) {
		if (sensors[i].id == sensor->id) {
			if (collisionEnd) {
				if (!sensors[i].changeable) sensors[i].changeable = true;
			}
			else {
				if (sensors[i].changeable) {
					if (sensors[i].active) {
						sensors[i].active = false;
						TraceLog(LOG_INFO, "ENEMY SENSOR %d NOT ACTIVE", sensors[i].id);

					}
					else {
						sensors[i].active = true;
						TraceLog(LOG_INFO, "ENEMY SENSOR %d ACTIVE", sensors[i].id);
					}

					sensors[i].changeable = false;

				}
			}
		}
		++i;
	}
}

void Enemy::CheckFinishLine() {
	finishedLap = true;
	int i = 0;
	while (i < sensors.size()) {
		if (!sensors[i].active) finishedLap = false;
		++i;
    }

	if (finishedLap) {
		Lap++;
		TraceLog(LOG_INFO, "AI CAR FINISHED LAP %d, STARTED LAP %d", Lap - 1, Lap);

		for (auto s : sensors) s.active = false;
	}
}