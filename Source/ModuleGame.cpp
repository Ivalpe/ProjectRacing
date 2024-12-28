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
	timer = 3, delayTimer = 60;
    timeInitial = true;
	timeMinus = 0;

	titleBG = LoadTexture("Assets/Main Menu/title screen.png");
	playBtTex = LoadTexture("Assets/Main Menu/Play Button.png");
	optBtTex = LoadTexture("Assets/Main Menu/Options Button.png");
	credBtTex = LoadTexture("Assets/Main Menu/Credits Button.png");


	onePlayerBtTex = LoadTexture("Assets/Main Menu/1Player Button.png");
	twoPlayersBtTex = LoadTexture("Assets/Main Menu/2Players Button.png");

	selectBG = LoadTexture("Assets/Main Menu/selection screen.png");
	nextBtTex = LoadTexture("Assets/Main Menu/siguiente.png");
	backBtTex = LoadTexture("Assets/Main Menu/atrás.png");

	bluePressX = LoadTexture("Assets/Main Menu/blue Press x to go back.png");
	bluePressZ = LoadTexture("Assets/Main Menu/blue Press z to confirm.png");
	redPressX = LoadTexture("Assets/Main Menu/red Press x to go back.png");
	redPressZ = LoadTexture("Assets/Main Menu/red Press z to confirm.png");

	useWASD = LoadTexture("Assets/Main Menu/blue use WASD.png");
	useArrows = LoadTexture("Assets/Main Menu/red use arrows.png");

	timer3 = LoadTexture("Assets/3.png");
	timer2 = LoadTexture("Assets/2.png");
	timer1 = LoadTexture("Assets/1.png");


	selectedVehicle = LoadTexture("Assets/selectVehicle.png");
	selectedVehicle2 = LoadTexture("Assets/selectVehicle2.png");

	gameMusic = LoadMusicStream("Assets/Audio/Music/In-game.ogg");
	charSelectMusic = LoadMusicStream("Assets/Audio/Music/Character select.ogg");
	mainMenuMusic = LoadMusicStream("Assets/Audio/Music/Main Menu.ogg");
	bep_fx = App->audio->LoadFx("Audio/Fx/bep timer.ogg");
	beep_fx = App->audio->LoadFx("Audio/Fx/beep timer.ogg");

	if (!mainMenuMusic.stream.buffer) {
		LOG("Failed to load main menu music");
	}

	if (!charSelectMusic.stream.buffer) {
		LOG("Failed to load character select music");
	}

	if (!gameMusic.stream.buffer) {
		LOG("Failed to load game music");
	}

	//Random Map
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 2);

	switch (dist6(rng))
	{
	case 1:
		App->map->Load("Assets/Maps/", "map1.tmx");
		mapLoaded = 1;
		distanceX = 32 * SCALE;
		distanceY = 49 * SCALE;
		pos = { 191 * SCALE, 296 * SCALE };
		initialY = pos.y;
		rot = LEFT_ANGLE;
		break;
	case 2:
		App->map->Load("Assets/Maps/", "map2.tmx");
		mapLoaded = 2;
		distanceX = -(32 * SCALE);
		distanceY = 49 * SCALE;
		pos = { 464 * SCALE, 24 * SCALE };
		initialY = pos.y;
		rot = RIGHT_ANGLE;
		break;
	default: //If this switch dont work load level 1
		App->map->Load("Assets/Maps/", "map1.tmx");
		mapLoaded = 1;
		distanceX = 32 * SCALE;
		distanceY = 49 * SCALE;
		pos = { 191 * SCALE, 296 * SCALE };
		initialY = pos.y;
		rot = LEFT_ANGLE;
		break;
	}

	Rectangle playBtPos = { 703, 239, 482, 149 };
	playButton = (GuiControlButton*)App->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "", playBtPos, this, { 0,0,0,0 }, &playBtTex);

	Rectangle onePlayerBtPos = { 738, 247, 436, 135 };
	onePlayerButton = (GuiControlButton*)App->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "", onePlayerBtPos, this, { 0,0,0,0 }, &onePlayerBtTex);


	Rectangle twoPlayersBtPos = { 738, 364, 436, 135 };
	twoPlayersButton = (GuiControlButton*)App->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "", twoPlayersBtPos, this, { 0,0,0,0 }, &twoPlayersBtTex);


	Rectangle nextBtPos = { 844, 580, 356, 111 };
	nextButton = (GuiControlButton*)App->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "", nextBtPos, this, { 0,0,0,0 }, &nextBtTex);


	Rectangle backBtPos = { 388, 580, 356, 111 };
	backButton = (GuiControlButton*)App->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "", backBtPos, this, { 0,0,0,0 }, &backBtTex);


	vehicles.push_back(LoadTexture("Assets/car1.png"));
	vehicles.push_back(LoadTexture("Assets/car2.png"));
	vehicles.push_back(LoadTexture("Assets/car3.png"));
	vehicles.push_back(LoadTexture("Assets/car4.png"));
	vehicles.push_back(LoadTexture("Assets/car5.png"));
	vehicles.push_back(LoadTexture("Assets/car6.png"));
	vehicles.push_back(LoadTexture("Assets/car7.png"));
	vehicles.push_back(LoadTexture("Assets/car8.png"));
	vehicles.push_back(LoadTexture("Assets/car9.png"));

	vehicleIcons.push_back(LoadTexture("Assets/Main Menu/Car Icons/mcqueen icon.png"));
	vehicleIcons.push_back(LoadTexture("Assets/Main Menu/Car Icons/marge icon.png"));
	vehicleIcons.push_back(LoadTexture("Assets/Main Menu/Car Icons/falcon icon.png"));
	vehicleIcons.push_back(LoadTexture("Assets/Main Menu/Car Icons/ken icon.png"));
	//vehicleIcons.push_back(LoadTexture("Assets/Main Menu/Car Icons/hamster icon.png"));
	vehicleIcons.push_back(LoadTexture("Assets/Main Menu/Car Icons/hamster icon.png"));
	vehicleIcons.push_back(LoadTexture("Assets/Main Menu/Car Icons/catbus icon.png"));
	vehicleIcons.push_back(LoadTexture("Assets/Main Menu/Car Icons/police cat icon.png"));
	vehicleIcons.push_back(LoadTexture("Assets/Main Menu/Car Icons/kamek2 icon.png"));
	vehicleIcons.push_back(LoadTexture("Assets/Main Menu/Car Icons/red potter icon.png"));

	items.push_back(LoadTexture("Assets/Item1.png"));





	car = DBG_NEW Player(App);
	if (TwoPlayerMode) car2 = DBG_NEW Player(App);
	for (auto i = 0; i < CARS; i++) {
		Enemy* enemyCar = DBG_NEW Enemy(App);
		enemyCars.push_back(enemyCar);
		ranking.push_back(enemyCar);
	}


	selectedPos = 0;
	selectedPosPlayer2 = 0;

	int width = SPRITE_WIDTH * 3;
	int height = SPRITE_HEIGHT * 3;
	float posX = 875, posY = 150;
	for (int i = 0; i < vehicles.size(); i++) {
		posVehicles.push_back(Vector2{ posX,posY });

		if ((i + 1) % 3 == 0) {
			posY += 125;
			posX = 875;
		}
		else posX += 125;
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
		delete car;
	}
	enemyCars.clear();
	car->CleanUp();
	delete car;

	car2->CleanUp();
	delete car2;

	App->map->CleanUp();

	UnloadMusicStream(mainMenuMusic);
	UnloadMusicStream(charSelectMusic);
	UnloadMusicStream(gameMusic);

	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{
	switch (stateGame)
	{
	case MAIN_MENU:
		if (App->audio->GetCurrentMusic().stream.buffer != mainMenuMusic.stream.buffer) {
			App->audio->PlayMusic(mainMenuMusic, 0.5f);
		}
		MainMenu();
		break;
	case SELECT_CHARACTER:
		if (App->audio->GetCurrentMusic().stream.buffer != charSelectMusic.stream.buffer) {
			App->audio->PlayMusic(charSelectMusic, 0.5f);
		}
		SelectCharacter();

		break;
	case GAME:
		if (App->audio->GetCurrentMusic().stream.buffer != gameMusic.stream.buffer) {
			App->audio->PlayMusic(gameMusic, 0.5f);
		}
		Game();
		break;

	default:

		break;
	}



	return UPDATE_CONTINUE;
}

void ModuleGame::MainMenu() {

	nextButton->active = false;
	backButton->active = false;

	Rectangle rect;
	rect.x = 0;
	rect.y = 0;
	rect.width = SCREEN_WIDTH;
	rect.height = SCREEN_HEIGHT;
	DrawTexture(titleBG, 0, 0, WHITE);


	if (!pressedPlay) {
		onePlayerButton->active = false;
		twoPlayersButton->active = false;
		playButton->active = true;
		OnGuiMouseClickEvent(playButton);
	}
	else {
		onePlayerButton->active = true;
		twoPlayersButton->active = true;
		playButton->active = false;
		OnGuiMouseClickEvent(onePlayerButton);
		OnGuiMouseClickEvent(twoPlayersButton);
	}
}

void ModuleGame::SelectCharacter() {

	DrawTexture(selectBG, 0, 0, WHITE);

	onePlayerButton->active = false;
	twoPlayersButton->active = false;

	nextButton->active = true;
	nextButton->state = GuiControlState::DISABLED;
	backButton->active = true;
	int cols = 3;

	if (!Player1Ready) {
		if (IsKeyPressed(KEY_RIGHT) && selectedPos % cols < cols - 1) selectedPos++;
		if (IsKeyPressed(KEY_LEFT) && selectedPos % cols > 0) selectedPos--;
		if (IsKeyPressed(KEY_UP) && selectedPos - cols >= 0) selectedPos -= cols;
		if (IsKeyPressed(KEY_DOWN) && selectedPos + cols <= vehicles.size() - 1) selectedPos += cols;
	}

	if (TwoPlayerMode && Player1Ready && !Player2Ready) {
		if (IsKeyPressed(KEY_D) && selectedPosPlayer2 % cols < cols - 1) selectedPosPlayer2++;
		if (IsKeyPressed(KEY_A) && selectedPosPlayer2 % cols > 0) selectedPosPlayer2--;
		if (IsKeyPressed(KEY_W) && selectedPosPlayer2 - cols >= 0) selectedPosPlayer2 -= cols;
		if (IsKeyPressed(KEY_S) && selectedPosPlayer2 + cols <= vehicles.size() - 1) selectedPosPlayer2 += cols;
	}

	if (TwoPlayerMode && Player1Ready) {
		DrawTexture(useWASD, 940, 20, WHITE);
		DrawTexture(bluePressZ, 880, 60, WHITE);
		DrawTexture(bluePressX, 900, 100, WHITE);
		DrawTexture(vehicleIcons[selectedPosPlayer2], 52, 0, WHITE);


	}
	else {
		DrawTexture(useArrows, 940, 20, WHITE);
		DrawTexture(redPressZ, 880, 60, WHITE);
		DrawTexture(redPressX, 900, 100, WHITE);
		DrawTexture(vehicleIcons[selectedPos], 52, 0, WHITE);
	}

	Rectangle rect;
	rect.x = 0;
	rect.y = 0;
	rect.width = SPRITE_WIDTH * SCALE;
	rect.height = SPRITE_HEIGHT * SCALE;
	for (int i = 0; i < vehicles.size(); i++) App->renderer->Draw(vehicles[i], posVehicles[i].x, posVehicles[i].y, &rect);

	//Select Vehicle
	App->renderer->Draw(selectedVehicle, posVehicles[selectedPos].x, posVehicles[selectedPos].y, &rect);
	if (TwoPlayerMode)	App->renderer->Draw(selectedVehicle2, posVehicles[selectedPosPlayer2].x, posVehicles[selectedPosPlayer2].y, &rect);


	OnGuiMouseClickEvent(backButton);

	if (TwoPlayerMode) {
		if (IsKeyPressed(KEY_Z)) {
			if (!Player1Ready) Player1Ready = true;
			else Player2Ready = true;
		}
		if (IsKeyPressed(KEY_X)) {
			if (Player1Ready && !Player2Ready) Player1Ready = false;
			else if (Player1Ready && Player2Ready) Player2Ready = false;
		}
	}
	else {
		if (IsKeyPressed(KEY_Z)) Player1Ready = true;
		if (IsKeyPressed(KEY_X) && Player1Ready) Player1Ready = false;
	}

	//Random Car
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, vehicles.size() - 1);
	if (TwoPlayerMode) {
		if (Player1Ready && Player2Ready) {
			nextButton->state = GuiControlState::NORMAL;
			nextButton->Update();
			OnGuiMouseClickEvent(nextButton);

			if (loadCars) {
				car->SetParameters(App->physics, vehicles[selectedPos], rot, items, 1);
				car->SetPosition(pos);
				pos.x += distanceX;
				pos.y += distanceY;

				car2->SetParameters(App->physics, vehicles[selectedPosPlayer2], rot, items, 2);
				car2->SetPosition(pos);
				pos.x += distanceX;
				pos.y = (pos.y == initialY ? pos.y + distanceY : pos.y - distanceY);

				for (auto car : enemyCars) {
					car->SetParameters(App->physics, vehicles[dist6(rng)], rot);
					car->SetPosition(pos);
					pos.x += distanceX;
					pos.y = (pos.y == initialY ? pos.y + distanceY : pos.y - distanceY);

				}

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
	}
	else if (Player1Ready) {

		nextButton->state = GuiControlState::NORMAL;
		nextButton->Update();
		OnGuiMouseClickEvent(nextButton);

		if (loadCars) {
			car->SetParameters(App->physics, vehicles[selectedPos], rot, items, 1);
			car->SetPosition(pos);
			pos.x += distanceX;
			pos.y += distanceY;

			for (auto car : enemyCars) {
				car->SetParameters(App->physics, vehicles[dist6(rng)], rot);
				car->SetPosition(pos);
				pos.x += distanceX;
				pos.y = (pos.y == initialY ? pos.y + distanceY : pos.y - distanceY);
			}


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
}

void ModuleGame::DrawUI() {
	
	if (timeInitial == true) {
		
		timeMinus = GetTime();
		timeInitial = false;
	}
	float spacing = 1.0f;
	Color color = BLACK;
	double timer = GetTime() - timeMinus;
	int timelap = car->Lap;
	

	App->renderer->DrawText(TextFormat("%.2f TIME", timer), SCREEN_WIDTH - 120, 30, GetFontDefault(), (int)spacing, color);
	App->renderer->DrawText(TextFormat("%d LAP", timelap), SCREEN_WIDTH - 120, 40, GetFontDefault(), (int)spacing, color);
	if (TwoPlayerMode) {
		int timelap2 = car2->Lap;
		App->renderer->DrawText(TextFormat("%d LAP", timelap2), SCREEN_WIDTH - 120, 50, GetFontDefault(), (int)spacing, color);
	}

}

void ModuleGame::Game() {


	nextButton->active = false;
	backButton->active = false;

	if (timer >= 0) delayTimer--;

	if (delayTimer <= 0 && timer >= 0) {
		delayTimer = 60;
		timer--;
	}


	switch (timer)
	{
	case 1:
		App->audio->PlayFx(bep_fx);
		DrawTexture(timer1, (SCREEN_WIDTH / 2) - (timer1.width / 2), (SCREEN_HEIGHT / 2) - (timer1.height / 2), WHITE);
		break;
	case 2:
		App->audio->PlayFx(bep_fx);
		DrawTexture(timer2, (SCREEN_WIDTH / 2) - (timer2.width / 2), (SCREEN_HEIGHT / 2) - (timer2.height / 2), WHITE);
		break;
	case 3:
		App->audio->PlayFx(beep_fx);
		DrawTexture(timer3, (SCREEN_WIDTH / 2) - (timer3.width / 2), (SCREEN_HEIGHT / 2) - (timer3.height / 2), WHITE);
		break;
	default:
		break;
	}

	car->Render();
	if (TwoPlayerMode) car2->Render();
	for (auto car : enemyCars) {
		car->Render();
	}


	if (timer <= 0) {
		car->Update();
		if (TwoPlayerMode) car2->Update();
		for (auto car : enemyCars) {
			car->Update();
		}
		DrawUI();
	}

	for (int i = 1; i < ranking.size(); ++i) {
		if (ranking[i - 1]->cpCount < ranking[i]->cpCount) {
			Entity* tempCar = ranking[i - 1];
			ranking[i - 1] = ranking[i];
			ranking[i] = tempCar;
		}
	}

	/*if(car->finishedLap) *//*car->PrintPosition(ranking);*/
	PrintRanking();

	DrawRectangleLines(App->renderer->camera.x, App->renderer->camera.y, SCREEN_WIDTH, SCREEN_HEIGHT, Color({ 0,0,255,255 }));



}

bool ModuleGame::OnGuiMouseClickEvent(GuiControl* control) {

	if (control->state == GuiControlState::PRESSED) {
		switch (control->id) {
		case 1:
			stateGame = MAIN_MENU;
			pressedPlay = true;
			break;
		case 2:
			stateGame = SELECT_CHARACTER;
			TwoPlayerMode = false;
			break;
		case 3:
			stateGame = SELECT_CHARACTER;
			TwoPlayerMode = true;
			break;
		case 4:
			stateGame = GAME;
			loadCars = true;
			break;
		case 5:
			stateGame = MAIN_MENU;
			break;
		}

		control->active = false;
	}

	return true;
}


void ModuleGame::PrintRanking() {
	for (int i = 0; i < ranking.size(); ++i) {
		const char* pilot;
		if (ranking[i]->carType == 0) pilot = "Player";
		else pilot = "AI";
	}
}
