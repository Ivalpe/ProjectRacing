#pragma once

#include "Module.h"
#include "GuiControl.h"

#include <list>

class GuiManager : public Module
{
public:

	// Constructor
	GuiManager(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~GuiManager();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	update_status Update() override;

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, Rectangle bounds, Module* observer, Rectangle sliderBounds = { 0,0,0,0 }, Texture2D* texture = nullptr);

public:

	std::list<GuiControl*> guiControlsList;
	Texture2D* texture;

};


