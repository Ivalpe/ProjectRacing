#pragma once

#include "Globals.h"
#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include "p2Point.h"

#include "raylib.h"
#include <vector>
#include "GuiControlButton.h"

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

	bool OnGuiMouseClickEvent(GuiControl* control) override;


	GameState stateGame;
	Texture2D titleBG, selectedVehicle, selectedVehicle2;
	Texture2D playBtTex, optBtTex, credBtTex;
	std::vector<Vector2> posVehicles;
	std::vector<Texture2D> vehicles;
	std::vector<Enemy*> enemyCars;
  
	int selectedPos;
	int selectedPosPlayer2;

	bool TwoPlayerMode = true;

	Player* car;
	bool Player1Ready = false;
	Player* car2;
	bool Player2Ready = false;
	std::vector<Object*> checkpoints;

private:

	GuiControlButton* playButton;
	
};
