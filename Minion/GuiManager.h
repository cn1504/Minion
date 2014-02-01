#pragma once

#include "StdInc.h"
#include "GuiComponent.h"
#include "GuiButton.h"
#include "GuiText.h"
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
	void onUpdate();
};

extern GuiManager GUI;

