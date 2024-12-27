
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

void Player::SetParameters(ModulePhysics* physics, Texture2D txt, float rot) {
	texture = txt;
	body = physics->CreateRectangle(0, 0, SPRITE_WIDTH * SCALE, SPRITE_HEIGHT * SCALE, b2_dynamicBody);

	body->body->SetTransform({ PIXEL_TO_METERS(x), PIXEL_TO_METERS(y) }, rot);
	body->listenerptr = this;
}

update_status Player::Update() {

	update_status ret = UPDATE_CONTINUE;

	currentSpeed = body->ScalarLinearVelocity();
	TraceLog(LOG_INFO, "speed = %.10f", currentSpeed);
	TraceLog(LOG_INFO, "angle: %f", GetBodyAngle());

	static b2Vec2 velocity = b2Vec2(0, 0);
	if (abs(currentSpeed) >= MinSpeed) {
		isSpinning = false;
		if (IsKeyDown(KEY_RIGHT) && GetBodyAngle() < MaxAngle) {
			if (isSpinningRight) TurnBody(forward, isSpinningRight, torqueSpeed, currentSpeed);
			isSpinningRight = true;
			isSpinning = true;
			TurnBody(forward, isSpinningRight, torqueSpeed, currentSpeed);
		}
		
		if (IsKeyDown(KEY_LEFT) && GetBodyAngle() > -MaxAngle) {
			if (!isSpinningRight) TurnBody(forward, isSpinningRight, torqueSpeed, currentSpeed);
			isSpinningRight = false;
			isSpinning = true;
			TurnBody(forward, isSpinningRight, torqueSpeed, currentSpeed);
		}

		if (!isSpinning) body->ResetAngularVelocity();
	}
	else body->ResetAngularVelocity();

	speed = 0;
	if (IsKeyDown(KEY_UP)) {
		stopped = false;
		forward = true;
		if ((int)currentSpeed * 10000 == 0) {
			TraceLog(LOG_INFO, "FASTER");
			if (currentSpeed <= MaxSpeed) speed -= forceIncrement * 2;
		}
		else if (currentSpeed <= MaxSpeed) speed -= forceIncrement;
	}
	else if (IsKeyDown(KEY_DOWN)) {
		stopped = false;
		forward = false;
		if ((int)currentSpeed * 10000 == 0) {
			TraceLog(LOG_INFO, "FASTER");
			if (currentSpeed <= MaxSpeed) speed += forceIncrement * 2;
		}
		else if (currentSpeed <= MaxSpeed) speed += forceIncrement;
	}


	if (IsKeyUp(KEY_UP) && IsKeyUp(KEY_DOWN)) {
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
	//SetPosition({ x + App->renderer->camera.x, y + App->renderer->camera.y });
	Rectangle source = { 0.0f , 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { x + App->renderer->camera.x, y + App->renderer->camera.y, (float)texture.width * SCALE , (float)texture.height * SCALE };
	Vector2 origin = { ((float)texture.width / (2.0f)) * SCALE, ((float)texture.height / (2.0f)) * SCALE };
	float rotation = body->GetRotation() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);


	return ret;
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
	} else {
		if (isGoingForward) FinalTorque = -torque / abs(speed);
		else FinalTorque = torque / abs(speed);
	}

	body->TurnWithTorque(FinalTorque);
}

void Entity::CheckSensor(PhysBody* sensor, bool collisionEnd) {
	for (auto s : sensors) {
		if (s->id == sensor->id) {
			if (collisionEnd) {
				if(!s->changeable) s->changeable = true;
			}
			else {
				if (s->changeable) {
					if (s->active) {
						s->active = false;
						TraceLog(LOG_INFO, "SENSOR %d NOT ACTIVE", s->id);

					}
					else {
						s->active = true;
						TraceLog(LOG_INFO, "SENSOR %d ACTIVE", s->id);
					}

					s->changeable = false;
				}
			}
		}
	}
}

void Entity::CheckFinishLine() {
	bool FinishedLap = true;
	for (auto s : sensors) {
		if (!s->active) FinishedLap = false;
	}
	
	if (FinishedLap) {
		Lap++;
		TraceLog(LOG_INFO, "FINISHED LAP, STARTED LAP %d", Lap);
		for (auto s : sensors) s->active = false;
	}
}