#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Player.h"
#include "Enemy.h"
#include "ModuleWindow.h"
#include "Map.h"

#include "GuiControl.h"
#include "GuiManager.h"

#include <random>


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
	

	titleBG = LoadTexture("Assets/Main Menu/title screen.png");
	playBtTex = LoadTexture("Assets/Main Menu/Play Button.png");
	optBtTex = LoadTexture("Assets/Main Menu/Options Button.png");
	credBtTex = LoadTexture("Assets/Main Menu/Credits Button.png");

	selectedVehicle = LoadTexture("Assets/selectVehicle.png");
	selectedVehicle2 = LoadTexture("Assets/selectVehicle2.png");
	App->map->Load("Assets/Maps/", "racing.tmx");

	Rectangle playBtPos = { 703, 239, 482, 149 };
	playButton = (GuiControlButton*)App->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1,"", playBtPos, this, {0,0,0,0}, &playBtTex);
	


	vehicles.push_back(LoadTexture("Assets/car1.png"));
	vehicles.push_back(LoadTexture("Assets/car2.png"));
	vehicles.push_back(LoadTexture("Assets/car3.png"));

	car = new Player(App);
	if(TwoPlayerMode) car2 = new Player(App);
	if (TwoPlayerMode) {
		for (auto i = 0; i < 6; i++) {
			enemyCars.push_back(new Enemy(App));
		}
	}
	else {
		for (auto i = 0; i < 7; i++) {
			enemyCars.push_back(new Enemy(App));
		}
	}

	selectedPos = 0;
	selectedPosPlayer2 = 0;

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
	LOG("Unloading scene");
	for (auto c : checkpoints) delete c;
	checkpoints.clear();

	LOG("Unloading Intro scene");
	for (auto car : enemyCars) {
		car->CleanUp();
	}
	car->CleanUp();
	App->map->CleanUp();


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
	rect.y = 0;
	rect.width = SCREEN_WIDTH;
	rect.height = SCREEN_HEIGHT;

	
	/*GuiControlType type, int id, const char* text, Rectangle bounds, Module* observer, Rectangle sliderBounds, Texture2D* texture*/

	App->renderer->Draw(titleBG, 0, 0, &rect);
	playButton->Update();
	DrawTexture(titleBG, 0, 0, WHITE);


	//TODO: Change to click button
	OnGuiMouseClickEvent(playButton);
		
	
}

void ModuleGame::SelectCharacter() {

	if (!Player1Ready) {
		if (IsKeyPressed(KEY_RIGHT) && selectedPos < vehicles.size() - 1) {
			selectedPos++;
		}

		if (IsKeyPressed(KEY_LEFT) && selectedPos > 0) {
			selectedPos--;
		}
	}

	if (TwoPlayerMode && !Player2Ready) {
		if (IsKeyPressed(KEY_D) && selectedPosPlayer2 < vehicles.size() - 1) {
			selectedPosPlayer2++;
		}

		if (IsKeyPressed(KEY_A) && selectedPosPlayer2 > 0) {
			selectedPosPlayer2--;
		}
	}


	Rectangle rect;
	rect.x = 0;
	rect.y = 0;
	rect.width = SPRITE_WIDTH * SCALE;
	rect.height = SPRITE_HEIGHT * SCALE;
	for (int i = 0; i < vehicles.size(); i++) App->renderer->Draw(vehicles[i], posVehicles[i].x, posVehicles[i].y, &rect);

	rect.x = 0;
	rect.y = 0;
	rect.width = SPRITE_WIDTH * SCALE;
	rect.height = SPRITE_HEIGHT * SCALE;
	App->renderer->Draw(selectedVehicle, posVehicles[selectedPos].x, posVehicles[selectedPos].y, &rect);
	if (TwoPlayerMode) App->renderer->Draw(selectedVehicle2, posVehicles[selectedPosPlayer2].x, posVehicles[selectedPosPlayer2].y, &rect);
	
	//Random Car
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, vehicles.size() - 1);

	Vector2 pos = { 185 * SCALE, 297 * SCALE };
	if (IsKeyPressed(KEY_SPACE)) Player1Ready = true;
	if (TwoPlayerMode && IsKeyPressed(KEY_Q)) Player2Ready = true;

	if (TwoPlayerMode) {
		if (Player1Ready && Player2Ready) {
			car->SetParameters(App->physics, vehicles[selectedPos]);
			car->SetPosition(pos);
			pos.x += 20 * SCALE;
			pos.y += 50 * SCALE;

			car2->SetParameters(App->physics, vehicles[selectedPosPlayer2], 2);
			car2->SetPosition(pos);
			pos.x += 20 * SCALE;
			pos.y += 50 * SCALE;

			for (auto car : enemyCars) {
				car->SetParameters(App->physics, vehicles[dist6(rng)]);
				car->SetPosition(pos);
				pos.x += 30 * SCALE;
				pos.y = (pos.y == 297 * SCALE ? pos.y + (50 * SCALE) : pos.y - (50 * SCALE));
			}

			stateGame = GAME;
			checkpoints = App->map->GetSensors();
			for (auto c : checkpoints) {
				if (!c->finishLine) {
					CheckpointSensor s;

					s.id = c->id;
					s.active = false;
					s.changeable = true;

					car->sensors.push_back(s);
					car2->sensors.push_back(s);
					for (auto e : enemyCars) e->sensors.push_back(s);
				}
			}
		}
	}
	else if (Player1Ready) {
		car->SetParameters(App->physics, vehicles[selectedPos]);
		car->SetPosition(pos);
		pos.x += 20 * SCALE;
		pos.y += 50 * SCALE;

		for (auto car : enemyCars) {
			car->SetParameters(App->physics, vehicles[dist6(rng)]);
			car->SetPosition(pos);
			pos.x += 30 * SCALE;
			pos.y = (pos.y == 297 * SCALE ? pos.y + (50 * SCALE) : pos.y - (50 * SCALE));
		}

		stateGame = GAME;
		checkpoints = App->map->GetSensors();
		for (auto c : checkpoints) {
			if (!c->finishLine) {
				CheckpointSensor s;

				s.id = c->id;
				s.active = false;
				s.changeable = true;

				car->sensors.push_back(s);
				for (auto e : enemyCars) e->sensors.push_back(s);
			}
		}
	}

}

void ModuleGame::Game() {

	car->Update();
	if(TwoPlayerMode) car2->Update();
	for (auto car : enemyCars) {
		car->Update();
	}

	//camera
	int carX, carY;
	/*car->GetPosition(carX, carY);
	car->body->GetPhysicPosition(carX, carY);*/
	//carX = METERS_TO_PIXELS(car->body->body->GetTransform().p.x);
	//carY = METERS_TO_PIXELS(car->body->body->GetTransform().p.y);

	//App->renderer->camera.x = ((carX - SCREEN_WIDTH / 2) * -(SCREEN_SIZE));
	//App->renderer->camera.y = ((carY - SCREEN_HEIGHT / 2) * -(SCREEN_SIZE));

	//TraceLog(LOG_INFO, "Postion %d, %d // Camera %f, %f", carX, carY, App->renderer->camera.x, App->renderer->camera.y);

	//just a test to check the received position was correct
	//DrawRectangle(carX - 25, carY - 40, 50, 80, Color({ 0,0,255,255 }));

	//drew the camera outline and yep, it encloses the map
	DrawRectangleLines(App->renderer->camera.x, App->renderer->camera.y, SCREEN_WIDTH, SCREEN_HEIGHT, Color({ 0,0,255,255 }));


}

bool ModuleGame::OnGuiMouseClickEvent(GuiControl* control) {

	if(control->id == 1 && control->state == GuiControlState::PRESSED)
	{
		stateGame = SELECT_CHARACTER;
		control->active = false;
	}
	return true;
}