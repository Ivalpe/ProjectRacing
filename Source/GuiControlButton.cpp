#include "GuiControlButton.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "Application.h"
#include "Module.h"
#include "Modulegame.h"


GuiControlButton::GuiControlButton(int id, Rectangle bounds, const char* text, Texture2D* texture = nullptr) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->texture = texture;

	canClick = true;
	drawBasic = false;
	this->active = false;
}

GuiControlButton::~GuiControlButton()
{

}

bool GuiControlButton::Update()
{
	if (state != GuiControlState::DISABLED)
	{
		// L16: TODO 3: Update the state of the GUiButton according to the mouse position
		Vector2 mousePos = GetMousePosition();

		//If the position of the mouse if inside the bounds of the button 
		if (mousePos.x > bounds.x && mousePos.x < bounds.x + bounds.width && mousePos.y > bounds.y && mousePos.y < bounds.y + bounds.height) {

			

			state = GuiControlState::FOCUSED;

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				state = GuiControlState::PRESSED;
			}

			if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}

	}

	switch (state)
	{
	case GuiControlState::DISABLED:
		currentTint = disabledTint;
		currentColor = disabledTint;
		break;
	case GuiControlState::NORMAL:

		currentTint = normalTint;
		currentColor = WHITE;
		break;

	case GuiControlState::FOCUSED:

		currentTint = focusedTint;
		currentColor = BLUE;
		break;

	case GuiControlState::PRESSED:
		currentTint = pressedTint;
		currentColor = DARKBLUE;
		break;
	}

	if (active) {
		if (texture != nullptr) {
			DrawTexture(*texture, bounds.x, bounds.y, currentColor);
		}
		else {
			DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, currentTint);
		}
	}

	return false;
}

//bool GuiControlButton::OnGuiMouseClickEvent(GuiControl* control)
//{
//	return true;
//}

