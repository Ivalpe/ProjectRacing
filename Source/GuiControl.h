#pragma once

//#include "ModuleInput.h"
#include "ModuleRender.h"
#include "Module.h"



enum class GuiControlType
{
	BUTTON,
	TOGGLE,
	CHECKBOX,
	SLIDER,
	SLIDERBAR,
	COMBOBOX,
	DROPDOWNBOX,
	INPUTBOX,
	VALUEBOX,
	SPINNER
};

enum class GuiControlState
{
	DISABLED,
	NORMAL,
	FOCUSED,
	PRESSED,
	SELECTED
};

class GuiControl
{
public:

	// Constructor
	GuiControl(GuiControlType type, int id) : type(type), id(id), state(GuiControlState::NORMAL) {}

	// Constructor
	GuiControl(GuiControlType type, Rectangle bounds, const char* text) :
		type(type),
		state(GuiControlState::NORMAL),
		bounds(bounds),
		text(text)
	{
		color.r = 255; color.g = 255; color.b = 255;
		texture = NULL;
	}

	// Called each loop iteration
	virtual bool Update()
	{
		return true;
	}

	// 
	void SetTexture(Texture2D* tex)
	{
		texture = tex;
		section = { 0, 0, 0, 0 };
	}

	// 
	void SetObserver(Module* module)
	{
		observer = module;
	}

	// 
	void NotifyObserver()
	{
		observer->OnGuiMouseClickEvent(this);
	}

public:

	int id;
	GuiControlType type;
	GuiControlState state;
	bool active;

	std::string text;           // Control text (if required)
	Rectangle bounds;        // Position and size
	Color color;        // Tint color

	Texture* texture;   // Texture atlas reference
	Rectangle section;       // Texture atlas base section

	Module* observer;        // Observer 
};
