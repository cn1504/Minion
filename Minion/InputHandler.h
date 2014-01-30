#pragma once
#include "StdInc.h"
#include "GuiButton.h"

class InputHandler
{
private:
	vector<GuiButton*> buttons;
	vector<GuiComponent*> components;

public:
	InputHandler();
	~InputHandler();

	void addGuiButton(GuiButton* button) { buttons.push_back(button); }
	void addGuiComponent(GuiComponent* comp) { components.push_back(comp); }

	void onUpdate(GLFWwindow* window, vec2 position);
	void onMouseLeftClick(GLFWwindow* window, vec2 position);
	void onMouseLeftDown(GLFWwindow* window, vec2 position);
};

extern InputHandler Input;