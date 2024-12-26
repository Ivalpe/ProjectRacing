#include "GuiManager.h"
#include "ModuleTextures.h"
#include "Application.h"
#include "GuiControlButton.h"
#include "ModuleAudio.h"
#include "Module.h"

GuiManager::GuiManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}

// L16: TODO 1: Implement CreateGuiControl function that instantiates a new GUI control and add it to the list of controls
GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, Rectangle bounds, Module* observer, Rectangle sliderBounds, Texture2D* texture)
{
	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = new GuiControlButton(id, bounds, text, texture);
		break;
	}

	//Set the observer
	guiControl->observer = observer;

	// Created GuiControls are add it to the list of controls
	guiControlsList.push_back(guiControl);

	return guiControl;
}

update_status GuiManager::Update()
{
	for (const auto& control : guiControlsList)
	{
		control->Update();
	}

	return UPDATE_CONTINUE;
}

bool GuiManager::CleanUp()
{
	for (const auto& control : guiControlsList)
	{
		delete control;
	}

	return true;
}



