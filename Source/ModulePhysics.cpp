#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleGame.h"

#include "p2Point.h"

#include <math.h>



ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;

	world = DBG_NEW b2World(b2Vec2(0, 0));
	world->SetContactListener(this);

	b2BodyDef bd;
	ground = world->CreateBody(&bd);
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

void PhysBody::GetPhysicPosition(int& x, int& y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x);
	y = METERS_TO_PIXELS(pos.y);
}

float PhysBody::GetRotation() const
{
	return body->GetAngle();
}

float PhysBody::GetAngleRotation() const
{
	return body->GetTransform().q.GetAngle();
}

float PhysBody::ScalarLinearVelocity() const {
	b2Vec2 currentvelocity = body->GetLinearVelocity();
	return (float)sqrt(currentvelocity.x * currentvelocity.x + currentvelocity.y * currentvelocity.y);
}

void PhysBody::ResetLinearVelocity() const {
	body->SetLinearVelocity(b2Vec2(0.f, 0.f));
}

void PhysBody::ResetAngularVelocity() const {
	body->SetAngularVelocity(0.f);
}

void PhysBody::ApplyMovingForce(float speed) const {
	b2Vec2 f = body->GetWorldVector(b2Vec2(0.0f, speed));
	body->ApplyForceToCenter(b2Vec2({ f.x, f.y }), true);
}

void PhysBody::TurnWithTorque(float torque) const {
	body->ApplyTorque(torque, true);
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
		if (f->GetShape()->TestPoint(body->GetTransform(), p)) return true;
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2Transform transform;
	transform.SetIdentity();

	b2RayCastInput input;
	input.p1 = { PIXEL_TO_METERS(x1),PIXEL_TO_METERS(y1) };
	input.p2 = { PIXEL_TO_METERS(x2),PIXEL_TO_METERS(y2) };
	input.maxFraction = 1.0f;
	int32 childIndex = 0;

	b2RayCastOutput output;
	for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
		if (f->GetShape()->RayCast(&output, input, body->GetTransform(), childIndex)) {
			normal_x = output.normal.x;
			normal_y = output.normal.y;

			float dx = (x2 - x1);
			float dy = (y2 - y1);
			float dist = sqrt((dx * dx) - (dy * dy));
			return (dist * output.fraction);
		}
	}

	return ret;
}

int ModulePhysics::RayCastGlobal(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y, b2Body* body) const {
	int ret = -1;

	b2Transform transform;
	transform.SetIdentity();

	b2RayCastInput input;
	input.p1 = { PIXEL_TO_METERS(x1),PIXEL_TO_METERS(y1) };
	input.p2 = { PIXEL_TO_METERS(x2),PIXEL_TO_METERS(y2) };
	input.maxFraction = 1.0f;
	int32 childIndex = 0;

	b2RayCastOutput output;
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
		if (b != body) {
			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
				if (f->GetShape()->RayCast(&output, input, b->GetTransform(), childIndex)) {
					normal_x = output.normal.x;
					normal_y = output.normal.y;

					float dx = (x2 - x1);
					float dy = (y2 - y1);
					float dist = sqrt((dx * dx) - (dy * dy));
					return (dist * output.fraction);
				}

			}
		}
	}

	return ret;
}


PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType bodyType)
{
	b2BodyDef body;
	body.type = bodyType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));


	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);
	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = DBG_NEW PhysBody();
	pbody->body = b;
	b->GetUserData().pointer = (uintptr_t)pbody;
	pbody->width = (int)(width * 0.5f);
	pbody->height = (int)(height * 0.5f);

	return pbody;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radious, b2BodyType type)
{
	// Create BODY at position x,y
	b2BodyDef body;

	//if (type == DYNAMIC) body.type = b2_dynamicBody;
	//if (type == STATIC) body.type = b2_staticBody;
	//if (type == KINEMATIC) body.type = b2_kinematicBody;
	body.type = type;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE
	b2CircleShape circle;
	circle.m_radius = PIXEL_TO_METERS(radious);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &circle;
	fixture.density = 1.0f;
	b->ResetMassData();

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Create our custom PhysBody class
	PhysBody* pbody = DBG_NEW PhysBody();
	pbody->body = b;
	b->GetUserData().pointer = (uintptr_t)pbody;
	pbody->width = radious * 0.5f;
	pbody->height = radious * 0.5f;

	// Return our PhysBody class
	return pbody;
}

PhysBody* ModulePhysics::CreateCircleSensor(int x, int y, int radious, b2BodyType type)
{
	// Create BODY at position x,y
	b2BodyDef body;

	//if (type == DYNAMIC) body.type = b2_dynamicBody;
	//if (type == STATIC) body.type = b2_staticBody;
	//if (type == KINEMATIC) body.type = b2_kinematicBody;
	body.type = type;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE
	b2CircleShape circle;
	circle.m_radius = PIXEL_TO_METERS(radious);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &circle;
	fixture.density = 1.0f;
	fixture.isSensor = true;
	b->ResetMassData();

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Create our custom PhysBody class
	PhysBody* pbody = DBG_NEW PhysBody();
	pbody->body = b;
	b->GetUserData().pointer = (uintptr_t)pbody;
	pbody->width = radious * 0.5f;
	pbody->height = radious * 0.5f;

	// Return our PhysBody class
	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, b2BodyType type)
{
	// Create BODY at position x,y
	b2BodyDef body;
	//if (type == DYNAMIC) body.type = b2_dynamicBody;
	//if (type == STATIC) body.type = b2_staticBody;
	//if (type == KINEMATIC) body.type = b2_kinematicBody;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Create our custom PhysBody class
	PhysBody* pbody = DBG_NEW PhysBody();
	pbody->body = b;
	//b->SetUserData(pbody);
	b->GetUserData().pointer = (uintptr_t)pbody;
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	// Return our PhysBody class
	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, b2BodyType type)
{
	// Create BODY at position x,y
	b2BodyDef body;
	//if (type == DYNAMIC) body.type = b2_dynamicBody;
	//if (type == STATIC) body.type = b2_staticBody;
	//if (type == KINEMATIC) body.type = b2_kinematicBody;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	// Add BODY to the world
	b2Body* b = world->CreateBody(&body);

	// Create SHAPE
	b2ChainShape shape;
	b2Vec2* p = DBG_NEW b2Vec2[size / 2];
	for (unsigned int i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	shape.CreateLoop(p, size / 2);

	// Create FIXTURE
	b2FixtureDef fixture;
	fixture.shape = &shape;

	// Add fixture to the BODY
	b->CreateFixture(&fixture);

	// Clean-up temp array
	delete p;

	// Create our custom PhysBody class
	PhysBody* pbody = DBG_NEW PhysBody();
	pbody->body = b;
	pbody->ctype = ColliderType::WALL;
	b->GetUserData().pointer = (uintptr_t)pbody;
	//b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	// Return our PhysBody class
	return pbody;
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	return true;
}

update_status ModulePhysics::PreUpdate()
{

	world->Step(1.f / 60.f, 6, 2);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{

	if (App->scene_intro->stateGame != GAME) {
		debug = false;
	}
	else if (IsKeyPressed(KEY_F1))
	{
		debug = !debug;
	}

	if (!debug)
	{
		return UPDATE_CONTINUE;
	}

	b2Body* mouseSelect = nullptr;
	Vector2 mousePosition = GetMousePosition();
	b2Vec2 pMousePosition = b2Vec2(PIXEL_TO_METERS(mousePosition.x), PIXEL_TO_METERS(mousePosition.y));

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->IsEnabled()) {
			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
			{
				switch (f->GetType())
				{
					// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();

					DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), (float)METERS_TO_PIXELS(shape->m_radius), Color{ 0, 0, 0, 128 });
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->m_count;
					b2Vec2 prev, v;

					for (int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->m_vertices[i]);
						if (i > 0)
							DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->m_vertices[0]);
					DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), RED);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for (int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if (i > 0)
							DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), GREEN);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), BLUE);
				}
				break;

				}

				if (mouse_joint == nullptr && mouseSelect == nullptr && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

					if (f->TestPoint(pMousePosition)) {

						mouseSelect = b;
						TraceLog(LOG_INFO, "MOUSE JOINT");
					}
				}
			}
		}
		
		
		
	}

	if (mouseSelect) {
		b2MouseJointDef def;
		
		def.bodyA = ground;
		def.bodyB = mouseSelect;
		def.target = pMousePosition;
		def.damping = 0.5f;
		def.stiffness = 20.f;
		def.maxForce = 100.f * mouseSelect->GetMass();

		mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
	}
	
	
	if (mouse_joint && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		mouse_joint->SetTarget(pMousePosition);
		b2Vec2 anchorPosition = mouse_joint->GetBodyB()->GetPosition();
		anchorPosition.x = METERS_TO_PIXELS(anchorPosition.x);
		anchorPosition.y = METERS_TO_PIXELS(anchorPosition.y);
		

		DrawLine(anchorPosition.x, anchorPosition.y, mousePosition.x, mousePosition.y, RED);
	}
	
	if (mouse_joint && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
	}


	return UPDATE_CONTINUE;
}





// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");
	delete world;
	world = NULL;

	return true;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	// Call the OnCollision listener function to bodies A and B, passing as inputs our custom PhysBody classes
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

	b2Filter disabledFilter;
	disabledFilter.categoryBits = 0;
	disabledFilter.maskBits = 0;



	if (physA && physA->listenerptr != NULL) {
		if (physB) // Ensure physB is also valid
		{
			physA->listenerptr->OnCollision(physA, physB);
		}
	}

	if (physB && physB->listenerptr != NULL) {
		if (physA) // Ensure physA is also valid
		{
			physB->listenerptr->OnCollision(physB, physA);
		}
	}
}

// Callback function to collisions with Box2D
void ModulePhysics::EndContact(b2Contact* contact)
{
	// Call the OnCollision listener function to bodies A and B, passing as inputs our custom PhysBody classes
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

	if (physA && physA->listenerptr != NULL) {
		if (physB) // Ensure physB is also valid
		{
			physA->listenerptr->OnCollisionEnd(physA, physB);
		}
	}

	if (physB && physB->listenerptr != NULL) {
		if (physA) // Ensure physA is also valid
		{
			physB->listenerptr->OnCollisionEnd(physB, physA);
		}
	}
}