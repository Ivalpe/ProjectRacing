
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

void Player::SetParameters(ModulePhysics* physics, Texture2D txt) {
	texture = txt;
	body = physics->CreateRectangle(0, 0, SPRITE_WIDTH * SCALE, SPRITE_HEIGHT * SCALE, b2_dynamicBody);
	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT / 2;

	body->body->SetTransform({ PIXEL_TO_METERS(x), PIXEL_TO_METERS(y) }, body->body->GetTransform().q.GetAngle());
	body->listenerptr = this;
}

update_status Player::Update() {

	update_status ret = UPDATE_CONTINUE;

	b2Vec2 currentvelocity = body->body->GetLinearVelocity();
	currentSpeed = (float)sqrt(currentvelocity.x * currentvelocity.x + currentvelocity.y * currentvelocity.y);
	TraceLog(LOG_INFO, "velocity: %.2f, %.2f, speed = %.10f", currentvelocity.x, currentvelocity.y, currentSpeed);

	float torqueSpeed = 0.7f * SCALE;
	static b2Vec2 velocity = b2Vec2(0, 0);
	if (abs(currentSpeed) >= 0.75) {
		isSpinning = false;
		if (IsKeyDown(KEY_RIGHT)) {
			if (forward) {
				if (isSpinningRight) body->body->ApplyTorque(torqueSpeed / abs(currentSpeed), true);
				body->body->ApplyTorque(torqueSpeed / abs(currentSpeed), true);
			}
			else {
				if (isSpinningRight) body->body->ApplyTorque(-torqueSpeed / abs(currentSpeed), true);
				body->body->ApplyTorque(-torqueSpeed / abs(currentSpeed), true);
			}
			isSpinningRight = false;
			isSpinning = true;
		}

		if (IsKeyDown(KEY_LEFT)) {
			if (forward) {
				if (!isSpinningRight) body->body->ApplyTorque(-torqueSpeed / abs(currentSpeed), true);
				body->body->ApplyTorque(-torqueSpeed / abs(currentSpeed), true);
			}
			else {
				if (!isSpinningRight) body->body->ApplyTorque(torqueSpeed / abs(currentSpeed), true);
				body->body->ApplyTorque(torqueSpeed / abs(currentSpeed), true);
			}
			isSpinningRight = true;
			isSpinning = true;
		}

		if (!isSpinning) body->body->SetAngularVelocity(0.f);
	}
	else body->body->SetAngularVelocity(0.f);

	float forceIncrement = 10.f;

	speed = 0;

	if (IsKeyDown(KEY_UP)) {
		Stopped = false;
		forward = true;
		if ((int)currentSpeed * 10000 == 0) {
			TraceLog(LOG_INFO, "FASTER");
			if (currentSpeed <= MaxSpeed) speed -= forceIncrement * 2;
		}
		else if (currentSpeed <= MaxSpeed) speed -= forceIncrement;
	}
	else if (IsKeyDown(KEY_DOWN)) {
		Stopped = false;
		forward = false;
		if ((int)currentSpeed * 10000 == 0) {
			TraceLog(LOG_INFO, "FASTER");
			if (currentSpeed <= MaxSpeed) speed += forceIncrement * 2;
		}
		else if (currentSpeed <= MaxSpeed) speed += forceIncrement;
	}


	if (IsKeyUp(KEY_UP) && IsKeyUp(KEY_DOWN)) {
		if (!Stopped) {
			if (currentSpeed <= 0.5) {
				Stopped = true;
				body->body->SetLinearVelocity(b2Vec2(0.f, 0.f));
			}
			else {
				if (forward) speed += forceIncrement / 2;
				else speed -= forceIncrement / 2;
			}
		}
		else body->body->SetLinearVelocity(b2Vec2(0.f, 0.f));
	}

	b2Vec2 f = body->body->GetWorldVector(b2Vec2(0.0f, speed));
	b2Vec2 p = body->body->GetWorldPoint(b2Vec2_zero);
	//body->body->SetLinearVelocity(b2Vec2({ f.x, f.y }));

	body->body->ApplyForceToCenter(b2Vec2({ f.x, f.y }), true);


	body->body->GetAngularVelocity();


	GetPosition(x, y);
	//x = body->body->GetTransform().p.x;
	//y = body->body->GetTransform().p.y;


	Rectangle source = { 0.0f , 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { x + App->renderer->camera.x , y + App->renderer->camera.y, (float)texture.width * SCALE , (float)texture.height * SCALE };
	Vector2 origin = { ((float)texture.width / (2.0f)) * SCALE, ((float)texture.height / (2.0f)) * SCALE };
	float rotation = body->GetRotation() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);


	//SetPosition({ (float)dest.x, (float)dest.y });

	return ret;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype) {
	case ColliderType::WALL:
		TraceLog(LOG_INFO, "COLLISION");
		Stopped = true;
		break;
	default:
		break;
	}
}

void Player::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype) {
	case ColliderType::WALL:
		break;
	default:
		break;
	}
}

void Entity::GetPosition(int& x, int& y) const {
	body->GetPhysicPosition(x, y);
}

void Entity::SetPosition(Vector2 pos) {

	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.x), PIXEL_TO_METERS(pos.y));
	body->body->SetTransform(bodyPos, body->GetRotation());

}