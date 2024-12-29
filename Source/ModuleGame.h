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
#include "Item.h"

class PhysBody;
class PhysicEntity;

enum GameState {
	MAIN_MENU,
	SELECT_CHARACTER,
	GAME,
	RACE_END
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
	void RaceEnd();
	void DrawUI();
	bool CleanUp();
	bool OnGuiMouseClickEvent(GuiControl* control) override;
	bool timeInitial;
	double timeMinus;


	void PrintRanking();


	GameState stateGame;
	Texture2D titleBG, selectBG, selectedVehicle, selectedVehicle2;
	Texture2D playBtTex, onePlayerBtTex, twoPlayersBtTex, optBtTex, credBtTex, nextBtTex, backBtTex;
	Texture2D bluePressZ, bluePressX, redPressZ, redPressX, useWASD, useArrows;
	Texture2D endRaceOnePlayer, endRaceTwoPlayers;

	Texture2D timer3, timer2, timer1;

	std::vector<Vector2> posVehicles;
	std::vector<Texture2D> vehicles;
	std::vector<Texture2D> vehicleIcons;
	std::vector<Enemy*> enemyCars;
	std::vector<Texture2D> items;
	std::vector<Item*>itemList;

	Item* itemBox;
  
	int selectedPos;
	int selectedPosPlayer2;

	bool pressedPlay = false;
	bool TwoPlayerMode = true;
	bool loadCars = false;

	bool FinishRace = false;
	int FinalRankingTracker = 0;

	bool PlayerOneDone = false;
	int PlayerOneFinalPos = 0;
	double PlayerOneFinalTime = 0.f;
	bool PlayerTwoDone = false;
	int PlayerTwoFinalPos = 0; 
	double PlayerTwoFinalTime = 0.f;

	Player* car;
	bool Player1Ready = false;
	Player* car2;
	bool Player2Ready = false;
	std::vector<Object*> checkpoints;
	std::vector<Entity*> ranking;
	std::vector<Music> musicGame;

	Font gamTex;
	float raceFontSize;
	float endFontSize;

private:

	GuiControlButton *playButton, *onePlayerButton, *twoPlayersButton, *nextButton, *backButton;
	int mapLoaded;
	float distanceX, distanceY, initialY, rot;
	Vector2 pos;
	int timer, delayTimer;

	Music charSelectMusic;
	Music mainMenuMusic;
	int bepSFX;
	int beepSFX;
	int songId;
	pugi::xml_document xmlFile;
	pugi::xml_parse_result result;
};
