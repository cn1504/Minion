#include "GuiButton.h"
#include "InputHandler.h"


GuiButton::GuiButton(GLuint x, GLuint y, GLuint width, GLuint height)
	: GuiComponent(x, y, width, height)
{
	Input.addGuiButton(this);

	enable();
}


GuiButton::~GuiButton()
{
}


void GuiButton::onClick(const ActionEvent& e)
{
	if (hidden || !enabled)
		return;

	for (auto listener : actionListeners) {
		listener(e);
	}
}
