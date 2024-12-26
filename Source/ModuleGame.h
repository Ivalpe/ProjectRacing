#pragma once

#include "Globals.h"
#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include "p2Point.h"

#include "raylib.h"
#include <vector>

class PhysBody;
class PhysicEntity;

enum GameState {
	MAIN_MENU,
	SELECT_CHARACTER,
	GAME
};

class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	void MainMenu();
	void SelectCharacter();
	void Game();
	bool CleanUp();

	GameState stateGame;
	Texture2D background_layer, background_layer2, background_layer3, selectedVehicle;

	std::vector<Texture2D> vehicles;
	std::vector<Vector2> posVehicles;
	std::vector<Enemy*> enemyCars;
	int selectedPos;

	Player* car;
	std::vector<Object*> checkpoints;

public:

	
};
