#pragma once
#include "GuiComponent.h"

class ActionEvent {
public:
	ActionEvent(GLFWwindow* window, vec2 mouse) { this->window = window, mousePosition = mouse; }
	virtual ~ActionEvent() {};

	vec2 mousePosition; 
	GLFWwindow* window;
};

class GuiButton :
	public GuiComponent
{
protected:
	vector<function<void(const ActionEvent&)>> actionListeners;
	bool enabled;

public:
	GuiButton(GLuint x, GLuint y, GLuint width, GLuint height);
	virtual ~GuiButton();

	void onClick(const ActionEvent& e);
	void addActionListener(function<void(const ActionEvent&)> func) { actionListeners.push_back(func); }

	void enable() { enabled = true; }
	void disable() { enabled = false; }
};

