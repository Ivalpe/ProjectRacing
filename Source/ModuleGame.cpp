#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Player.h"

ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	stateGame = MAIN_MENU;
	background_layer = LoadTexture("Assets/background.png");
	background_layer2 = LoadTexture("Assets/background2.png");
	background_layer3 = LoadTexture("Assets/background3.png");
	selectedVehicle = LoadTexture("Assets/selectVehicle.png");


	vehicles.push_back(LoadTexture("Assets/car1.png"));
	vehicles.push_back(LoadTexture("Assets/car2.png"));
	vehicles.push_back(LoadTexture("Assets/car3.png"));

	car = new Player();
	car->SetParameters(App->physics, vehicles[0]);

	selectedPos = 0;
	
	int width = SPRITE_WIDTH * 3;
	int height = SPRITE_HEIGHT * 3;
	float posX = 100, posY = 100;
	for (int i = 0; i < vehicles.size(); i++) {
		posVehicles.push_back(Vector2{ posX,posY });

		if (i > 0 && i % 10 == 0) {
			posY += 100;
			posX = 100;
		}
		else posX += 100;
	}

	

	return ret;
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{
	switch (stateGame)
	{
	case MAIN_MENU:
		MainMenu();
		break;
	case SELECT_CHARACTER:
		SelectCharacter();
		break;
	case GAME:
		Game();
		break;
	}


	return UPDATE_CONTINUE;
}

void ModuleGame::MainMenu() {
	Rectangle rect;
	rect.x = 0;
	rect.y = 208;
	rect.width = SCREEN_WIDTH;
	rect.height = SCREEN_HEIGHT;
	App->renderer->Draw(background_layer, 0, 0, &rect);


	if (IsKeyPressed(KEY_SPACE)) {
		stateGame = SELECT_CHARACTER;
	}
}

void ModuleGame::SelectCharacter() {

	if (IsKeyPressed(KEY_RIGHT) && selectedPos < vehicles.size() - 1) {
		selectedPos++;
	}

	if (IsKeyPressed(KEY_LEFT) && selectedPos > 0) {
		selectedPos--;
	}

	Rectangle rect;
	rect.x = 0;
	rect.y = 208;
	rect.width = SCREEN_WIDTH;
	rect.height = SCREEN_HEIGHT;
	App->renderer->Draw(background_layer2, 0, 0, &rect);

	rect.x = 0;
	rect.y = 0;
	rect.width = SPRITE_WIDTH * 3;
	rect.height = SPRITE_HEIGHT * 3;
	for (int i = 0; i < vehicles.size(); i++) App->renderer->Draw(vehicles[i], posVehicles[i].x, posVehicles[i].y, &rect);

	rect.x = 0;
	rect.y = 0;
	rect.width = SPRITE_WIDTH * 3;
	rect.height = SPRITE_HEIGHT * 3;
	App->renderer->Draw(selectedVehicle, posVehicles[selectedPos].x, posVehicles[selectedPos].y, &rect);

	if (IsKeyPressed(KEY_SPACE)) {
		stateGame = GAME;
	}
	
}

void ModuleGame::Game() {
	Rectangle rect;
	rect.x = 0;
	rect.y = 208;
	rect.width = SCREEN_WIDTH;
	rect.height = SCREEN_HEIGHT;
	App->renderer->Draw(background_layer3, 0, 0, &rect);

	car->Update();
}
