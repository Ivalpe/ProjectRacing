
#include "Globals.h"
#include "Entity.h"
#include "Module.h"
#include "Player.h"
#include "box2d/b2_math.h"
#include <algorithm>


Player::Player(Application* parent) : Entity(parent)
{
	speed = 0.f;
}


void Player::SetParameters(ModulePhysics* physics, Texture2D txt, float rot, int player) {

	texture = txt;
	body = physics->CreateRectangle(0, 0, SPRITE_WIDTH * SCALE, SPRITE_HEIGHT * SCALE, b2_dynamicBody);
	carType = PLAYER;

	if (player == 1) {
		TurnLeft = KEY_LEFT;
		TurnRight = KEY_RIGHT;
		MoveBack = KEY_DOWN;
		MoveForward = KEY_UP;

		body->body->SetTransform({ PIXEL_TO_METERS(x), PIXEL_TO_METERS(y) }, rot);
	}
	else {
		TurnLeft = KEY_A;
		TurnRight = KEY_D;
		MoveBack = KEY_S;
		MoveForward = KEY_W;

		body->body->SetTransform({ PIXEL_TO_METERS(x), PIXEL_TO_METERS(y) }, rot);
	}

	body->listenerptr = this;
}

update_status Player::Update() {

	update_status ret = UPDATE_CONTINUE;

	currentSpeed = body->ScalarLinearVelocity();
	/*TraceLog(LOG_INFO, "speed = %.10f", currentSpeed);
	TraceLog(LOG_INFO, "angle: %f", GetBodyAngle());*/

	static b2Vec2 velocity = b2Vec2(0, 0);
	if (abs(currentSpeed) >= MinSpeed) {
		isSpinning = false;
		if (IsKeyDown(TurnRight) && GetBodyAngle() < MaxAngle) {
			if (isSpinningRight) TurnBody(forward, isSpinningRight, torqueSpeed, currentSpeed);
			isSpinningRight = true;
			isSpinning = true;
			TurnBody(forward, isSpinningRight, torqueSpeed, currentSpeed);
		}

		if (IsKeyDown(TurnLeft) && GetBodyAngle() > -MaxAngle) {
			if (!isSpinningRight) TurnBody(forward, isSpinningRight, torqueSpeed, currentSpeed);
			isSpinningRight = false;
			isSpinning = true;
			TurnBody(forward, isSpinningRight, torqueSpeed, currentSpeed);
		}

		if (!isSpinning) body->ResetAngularVelocity();
	}
	else body->ResetAngularVelocity();

	speed = 0;
	if (IsKeyDown(MoveForward)) {
		stopped = false;
		forward = true;
		if ((int)currentSpeed * 10000 == 0) {
			TraceLog(LOG_INFO, "FASTER");
			if (currentSpeed <= MaxSpeed) speed -= forceIncrement * 2;
		}
		else if (currentSpeed <= MaxSpeed) speed -= forceIncrement;
	}
	else if (IsKeyDown(MoveBack)) {
		stopped = false;
		forward = false;
		if ((int)currentSpeed * 10000 == 0) {
			TraceLog(LOG_INFO, "FASTER");
			if (currentSpeed <= MaxSpeed) speed += forceIncrement * 2;
		}
		else if (currentSpeed <= MaxSpeed) speed += forceIncrement;
	}


	if (IsKeyUp(MoveForward) && IsKeyUp(MoveBack)) {
		if (!stopped) {
			if (currentSpeed <= MinSpeed) {
				stopped = true;
				body->ResetLinearVelocity();
			}
			else {
				if (forward) speed += forceIncrement / 2;
				else speed -= forceIncrement / 2;
			}
		}
		else body->ResetLinearVelocity();
	}

	body->ApplyMovingForce(speed);

	GetPosition(x, y);
	return ret;
}

void Player::Render() {
	GetPosition(x, y);
	Rectangle source = { 0.0f , 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { x + App->renderer->camera.x, y + App->renderer->camera.y, (float)texture.width * SCALE , (float)texture.height * SCALE };
	Vector2 origin = { ((float)texture.width / (2.0f)) * SCALE, ((float)texture.height / (2.0f)) * SCALE };
	float rotation = body->GetRotation() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype) {
	case ColliderType::WALL:
		TraceLog(LOG_INFO, "COLLISION");
		stopped = true;
		break;
	case ColliderType::SENSOR:
		CheckSensor(physB, false);

		break;
	case ColliderType::FINISH_LINE:
		CheckSensor(physB, false);
		CheckFinishLine();

		break;
	default:
		break;
	}
}

void Player::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
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

// Entity -----------------------------------------

void Entity::GetPosition(int& x, int& y) const {
	body->GetPhysicPosition(x, y);
}

void Entity::SetPosition(Vector2 pos) {

	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y));
	body->body->SetTransform(bodyPos, body->GetRotation());

}

float Entity::GetBodyAngle() const {
	return body->GetAngleRotation();

}

void Entity::TurnBody(bool isGoingForward, bool isGoingRight, float torque, float speed) const {
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

void Player::CheckSensor(PhysBody* sensor, bool collisionEnd) {
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
						TraceLog(LOG_INFO, "SENSOR %d NOT ACTIVE", sensors[i].id);

					}
					else {
						sensors[i].active = true;
						TraceLog(LOG_INFO, "SENSOR %d ACTIVE", sensors[i].id);
					}

					sensors[i].changeable = false;

				}
			}
		}
		++i;
	}
}

void Player::CheckFinishLine() {
	finishedLap = true;
	int i = 0;
	while (i < sensors.size()) {
		if (!sensors[i].active) finishedLap = false;
		++i;
	}

	if (finishedLap) {
		Lap++;

		TraceLog(LOG_INFO, "PLAYER FINISHED LAP % d, STARTED LAP % d", Lap - 1, Lap);
		for (auto s : sensors) s.active = false;
	}

}

void Player::PrintPosition(std::vector<Entity*> ranking) {
	for (int i = 0; i < ranking.size(); ++i) {
		if (ranking[i]->carType == PLAYER) {
			TraceLog(LOG_INFO, "position: %d/%d", i + 1, ranking.size());
		}
	}
}