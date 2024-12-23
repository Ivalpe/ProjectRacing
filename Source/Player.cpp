
#include "Globals.h"
#include "Entity.h"
#include "Player.h"
#include <algorithm>


Player::Player() : Entity()
{
	speed = 0.f;
}

void Player::SetParameters(ModulePhysics* physics, Texture2D txt) {
	texture = txt;
	body = physics->CreateRectangle(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT, b2_dynamicBody);
	x = 300;
	y = 300;

	body->body->SetTransform({ PIXEL_TO_METERS(x), PIXEL_TO_METERS(y) }, body->body->GetTransform().q.GetAngle());
}

void Player::Update() {
	static b2Vec2 velocity = b2Vec2(0, 0);
	if (abs(speed) >= 1) {
		isSpinning = false;
		if (IsKeyDown(KEY_RIGHT)) {
			if (isSpinningRight) body->body->ApplyTorque(0.09f / abs(speed), true);
			body->body->ApplyTorque(0.02f / abs(speed), true);
			isSpinningRight = false;
			isSpinning = true;
		}
		
		if (IsKeyDown(KEY_LEFT)) {
			if(!isSpinningRight) body->body->ApplyTorque(-0.09f / abs(speed), true);
			body->body->ApplyTorque(-0.02f / abs(speed), true);
			isSpinningRight = true;
			isSpinning = true;
		}

		if (!isSpinning) body->body->SetAngularVelocity(0.f);
	}
	else body->body->SetAngularVelocity(0.f);

	if (IsKeyDown(KEY_UP)) {
		if (speed >= -MaxSpeed) speed -= 0.1f;
	}
	else if (IsKeyDown(KEY_DOWN)) {
		if (speed <= MaxSpeed) speed += 0.1f;
	}


	if (IsKeyUp(KEY_UP) && IsKeyUp(KEY_DOWN)) {
		if (speed > 0) {
			speed -= 0.1;
		}
		else if (speed < 0) {
			speed += 0.1;
		}
	}

	b2Vec2 f = body->body->GetWorldVector(b2Vec2(0.0f, speed));
	b2Vec2 p = body->body->GetWorldPoint(b2Vec2_zero);
	body->body->SetLinearVelocity(b2Vec2({ f.x, f.y }));


	body->body->GetAngularVelocity();
	

	//TraceLog(LOG_INFO, "Position: %f, WV (%.2f, %.2f), velocity: (%.2f, %.2f), Force: f", x, f.x, f.y, body->body->GetLinearVelocity().x, body->body->GetLinearVelocity().y);

	GetPosition(x, y);
	Vector2 position = { x,y };

	
	Rectangle source = { 0.0f , 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { position.x , position.y , (float)texture.width * scale , (float)texture.height * scale };
	Vector2 origin = { ((float)texture.width / (2.0f)) * scale, ((float)texture.height / (2.0f)) * scale };
	float rotation = body->GetRotation() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);



}

void Entity::GetPosition(int& x, int& y) const {
	body->GetPhysicPosition(x, y);
}