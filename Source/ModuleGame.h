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
	void DrawUI();
	bool CleanUp();
	bool OnGuiMouseClickEvent(GuiControl* control) override;

	void PrintRanking();


	GameState stateGame;
	Texture2D titleBG, selectBG, selectedVehicle, selectedVehicle2;
	Texture2D playBtTex, onePlayerBtTex, twoPlayersBtTex, optBtTex, credBtTex, nextBtTex, backBtTex;
	Texture2D bluePressZ, bluePressX, redPressZ, redPressX, useWASD, useArrows;

	Texture2D timer3, timer2, timer1;

	std::vector<Vector2> posVehicles;
	std::vector<Texture2D> vehicles;
	std::vector<Texture2D> vehicleIcons;
	std::vector<Enemy*> enemyCars;



  
	int selectedPos;
	int selectedPosPlayer2;

	bool pressedPlay = false;
	bool TwoPlayerMode = true;
	bool loadCars = false;



	Player* car;
	bool Player1Ready = false;
	Player* car2;
	bool Player2Ready = false;
	std::vector<Object*> checkpoints;
	std::vector<Entity*> ranking;

private:

	GuiControlButton *playButton, *onePlayerButton, *twoPlayersButton, *nextButton, *backButton;
	int mapLoaded;
	float distanceX, distanceY, initialY, rot;
	Vector2 pos;
	int timer, delayTimer;

	Music gameMusic;
	Music charSelectMusic;
	Music mainMenuMusic;
};
