
#include "Globals.h"
#include "Entity.h"
#include "Module.h"
#include "Enemy.h"
#include "box2d/b2_math.h"
#include <random>

Enemy::Enemy(Application* parent) : Entity(parent)
{
	speed = 0.f;

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> randomSpeed(3, 7);
	MaxSpeed = (randomSpeed(rng));
}

void Enemy::SetParameters(ModulePhysics* physics, Texture2D txt, float rot) {
	texture = txt;
	body = physics->CreateRectangle(0, 0, SPRITE_WIDTH * SCALE, SPRITE_HEIGHT * SCALE, b2_dynamicBody);

	dc = Direction::DISABLE;

	carType = AI;

	body->body->SetTransform({ PIXEL_TO_METERS(x), PIXEL_TO_METERS(y) }, rot);
	body->ctype = ColliderType::CAR;
	body->listenerptr = this;
}

bool Enemy::TurnRight(float initialAngle, float finalAngle) {

	initialAngle = fmod(initialAngle, 2 * PI);
	if (initialAngle < 0) initialAngle += 2 * PI;

	finalAngle = fmod(finalAngle, 2 * PI);
	if (finalAngle < 0) finalAngle += 2 * PI;

	float dif = finalAngle - initialAngle;

	if (dif < -PI) {
		dif += 2 * PI;
	}
	else if (dif >= PI) {
		dif -= 2 * PI;
	}

	return dif < 0;
}

update_status Enemy::Update() {

	update_status ret = UPDATE_CONTINUE;

	static b2Vec2 velocity = b2Vec2(0, 0);
	currentSpeed = body->ScalarLinearVelocity();

	if (abs(currentSpeed) >= MinSpeed) {
		isSpinning = false;
		float up = UP_ANGLE;
		float right = RIGHT_ANGLE;
		float down = DOWN_ANGLE;
		float left = LEFT_ANGLE;
		if (dc == Direction::UP && body->GetAngleRotation() != up) {
			isSpinning = true;
			TurnBody(forward, TurnRight(body->GetAngleRotation(), up), torqueSpeed * 3, currentSpeed);
		}
		else if (dc == Direction::RIGHT && body->GetAngleRotation() != right) {
			isSpinning = true;
			TurnBody(forward, TurnRight(body->GetAngleRotation(), right), torqueSpeed * 3, currentSpeed);
		}
		else if (dc == Direction::DOWN && body->GetAngleRotation() != down) {
			isSpinning = true;
			TurnBody(forward, TurnRight(body->GetAngleRotation(), down), torqueSpeed * 3, currentSpeed);
		}
		else if (dc == Direction::LEFT && body->GetAngleRotation() != left) {
			isSpinning = true;
			TurnBody(forward, TurnRight(body->GetAngleRotation(), left), torqueSpeed * 3, currentSpeed);
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
		stopped = true;
		break;
	case ColliderType::SENSOR:
		CheckSensor(physB, false);
		break;
	case ColliderType::FINISH_LINE:
		CheckFinishLine();
		CheckSensor(physB, true);
		break;
	case ColliderType::UP:
		dc = Direction::UP;
		break;
	case ColliderType::RIGHT:
		dc = Direction::RIGHT;
		break;
	case ColliderType::DOWN:
		dc = Direction::DOWN;
		break;
	case ColliderType::LEFT:
		dc = Direction::LEFT;
		break;
	default:
		break;
	}
}

void Enemy::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype) {
	case ColliderType::SENSOR:
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


void Enemy::CheckSensor(PhysBody* sensor, bool collisionEnd) {
	int i = 0;
	while (i < sensors.size()) {
		if (sensors[i].id == sensor->id) {
			if (collisionEnd) {
				if (!sensors[i].changeable) sensors[i].changeable = true;
			}
			else {
				if (sensors[i].changeable) {
					/*if (sensors[i].active) sensors[i].active = false;
					else */sensors[i].active = true;
					/*sensors[i].changeable = false;*/
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
		for (auto s : sensors) s.active = false;
		if (Lap >= MaxLaps) {
			FinishedLaps = true;
			EndedRace = true;
		}
	}
}