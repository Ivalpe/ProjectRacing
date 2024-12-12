
#include "Globals.h"
#include "Entity.h"
#include "Player.h"


Player::Player() : Entity()
{
}

void Player::SetParameters(ModulePhysics* physics, Texture2D txt) {
	texture = txt;
	body = physics->CreateRectangle(0, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
	position.Set(100, 100);
}

void Player::Update() {
	b2Vec2 velocity = b2Vec2(0, 0);
	if (IsKeyPressed(KEY_RIGHT)){
		velocity.Set(1000, 0);
	}

	body->body->SetLinearVelocity({10, 10});

	b2Transform pbodyPos = body->body->GetTransform();
	position.Set(METERS_TO_PIXELS(pbodyPos.p.x) - 32 / 2, METERS_TO_PIXELS(pbodyPos.p.y) - 16 / 2);

	//int x, y;
	//body->body.GetPhysicPosition(x, y);
	float scale = 1.6f;
	Rectangle source = { 0.0f , 0.0f, (float)texture.width, (float)texture.height };
	Rectangle dest = { position.x , position.y , (float)texture.width * scale , (float)texture.height * scale };
	Vector2 origin = { ((float)texture.width / (2.0f)) * scale, ((float)texture.height / (2.0f)) * scale };
	float rotation = body->GetRotation() * RAD2DEG;
	DrawTexturePro(texture, source, dest, origin, rotation, WHITE);



}