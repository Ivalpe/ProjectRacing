#pragma once
#include "Module.h"
#include "GuiControl.h"


class GuiControlButton : public GuiControl
{

public:

	GuiControlButton(int id, Rectangle bounds, const char* text, Texture2D* tex);
	virtual ~GuiControlButton();

	// Called each loop iteration
	bool Update();


private:
	Color normalTint = { 50, 86, 255, 255 };
	Color disabledTint = { 125, 147, 221, 255 };
	Color focusedTint = { 239, 94, 202, 255 };
	Color pressedTint = { 205, 0, 157, 255 };
	Color currentColor;
	Color currentTint;
	bool canClick = true;
	bool drawBasic = false;

	

};

#pragma once
