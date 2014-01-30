#pragma once

#include "StdInc.h"
#include "GuiButton.h"
#include "GuiComponent.h"
#include "Renderer.h"

class GuiManager
{
private:
	vector<GuiComponent*> components;

public:
	GuiManager();
	~GuiManager();

	void loadComponents(Renderer* renderer);
	void addComponent(GuiComponent* comp) { components.push_back(comp); }

	void onWindowResize();
};

extern GuiManager GUI;

