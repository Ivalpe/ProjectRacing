#pragma once

#include "Globals.h"
#include "Timer.h"
#include <vector>

#ifdef _DEBUG
#define DBG_NEW new (_NORMAL_BLOCK , __FILE__ , __LINE__)
#else
#define DBG_NEW new
#endif


class Module;
class ModuleWindow;
class ModuleRender;
class ModuleTextures;
class ModuleAudio;
class Map;
class ModulePhysics;
class ModuleGame;
class GuiManager;

class Application
{
public:

	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleAudio* audio;
	Map* map;
	ModulePhysics* physics;
	ModuleGame* scene_intro;
	GuiManager* guiManager;

private:

	std::vector<Module*> list_modules;
    uint64 frame_count = 0;

	Timer ptimer;
	Timer startup_time;
	Timer frame_time;
	Timer last_sec_frame_time;

	uint32 last_sec_frame_count = 0;
	uint32 prev_last_sec_frame_count = 0;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* module);
};