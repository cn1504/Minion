#include "InputHandler.h"


InputHandler Input;


InputHandler::InputHandler()
{
}


InputHandler::~InputHandler()
{
}


void InputHandler::onUpdate(GLFWwindow* window, vec2 position) 
{
	for (auto comp : components) {

		vec2 pos = comp->getPosition();
		vec2 dim = comp->getDimensions();
				
		if (comp->mouseOver) {
			// Check for mouse out event
			if (position.x < pos.x || position.x > pos.x + dim.x ||
				position.y < pos.y || position.y > pos.y + dim.y) {
				comp->mouseOver = false;
				MouseEvent me(window, position);
				comp->onMouseOut(me);
			}
		}
		else {
			// Check for mouse over event
			if (position.x > pos.x && position.x < pos.x + dim.x &&
				position.y > pos.y && position.y < pos.y + dim.y) {
				comp->mouseOver = true;
				MouseEvent me(window, position);
				comp->onMouseIn(me);				
			}
		}

		if (comp->dragged) {
			MouseEvent me(window, position);
			comp->onMouseClickAndDrag(me);
		}
	}
}


void InputHandler::onMouseLeftClick(GLFWwindow* window, vec2 position) 
{
	// cout << "Left Click Position: (" << position.x << ", " << position.y << ")" << endl;
	// cout << "Component Count: " << components.size() << endl;
	for (auto button : buttons)
	{
		vec2 pos = button->getPosition();
		vec2 dim = button->getDimensions();
		if (position.x > pos.x && position.x < pos.x + dim.x) {
			if (position.y > pos.y && position.y < pos.y + dim.y) {
				// click on button
				ActionEvent ae(window, position);
				button->onClick(ae);
			}
		}
	}

	// Reset all drags
	MouseEvent me(window, position);
	for (auto comp : components)
	{
		comp->onMouseClickRelease(me);
	}
}


void InputHandler::onMouseLeftDown(GLFWwindow* window, vec2 position) {
	for (auto comp : components) {

		if (comp->mouseOver) {
			MouseEvent me(window, position);
			comp->onMouseClickAndDrag(me);
		}

	}
}