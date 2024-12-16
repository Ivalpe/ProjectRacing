
#include "Globals.h"
#include "Entity.h"
#include "Player.h"


Player::Player() : Entity()
{
}

void Player::SetParameters(ModulePhysics* physics, Texture2D txt) {
	texture = txt;
	body = physics->CreateRectangle(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
	x = 300;
	y = 300;

	body->body->SetTransform({ PIXEL_TO_METERS(x), PIXEL_TO_METERS(y) }, body->body->GetTransform().q.GetAngle());
}

void Player::Update() {
	static b2Vec2 velocity = b2Vec2(0, 0);

	if (IsKeyDown(KEY_RIGHT)) body->body->ApplyTorque(10.0f, true);
	else if (IsKeyDown(KEY_LEFT)) body->body->ApplyTorque(-10.0f, true);

	if (IsKeyDown(KEY_UP)) {
		b2Vec2 f = body->body->GetWorldVector(b2Vec2(0.0f, -50.0f));
		b2Vec2 p = body->body->GetWorldPoint(b2Vec2_zero);
		body->body->ApplyForce(f, p, true);

	} else if (IsKeyDown(KEY_DOWN)) {
		velocity.y -= 10.f;
	}

	TraceLog(LOG_INFO, "Position: %f, velocity: %f, Force: %f", x, body->body->GetLinearVelocity().y, body->body->GetAngle());
	//body->body->ApplyForceToCenter({ 200, 100 }, true);

	//body->body->ApplyForce({ 0.f, 10.f }, b2Vec2_zero, true);
	//body->body->SetLinearVelocity(velocity);
	//body->body->SetTransform(body->body->GetPosition(), body->body->GetTransform().q.GetAngle());
	//body->body->ApplyForceToCenter()

	GetPosition(x, y);
	Vector2 position = { x,y };

	//b2Transform pbodyPos = body->body->GetTransform();
	//position.Set(METERS_TO_PIXELS(pbodyPos.p.x) - 32 / 2, METERS_TO_PIXELS(pbodyPos.p.y) - 16 / 2);

	//int x, y;
	//body->body.GetPhysicPosition(x, y);
	float scale = 1.6f;
	Rectangle source = { 0.0f , 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { position.x , position.y , (float)texture.width * scale , (float)texture.height * scale };
	Vector2 origin = { ((float)texture.width / (2.0f)) * scale, ((float)texture.height / (2.0f)) * scale };
	float rotation = body->GetRotation() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);



}

void Entity::GetPosition(int& x, int& y) const {
	body->GetPhysicPosition(x, y);
}