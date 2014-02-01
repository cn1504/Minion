#include "GuiManager.h"


GuiManager GUI;


GuiManager::GuiManager()
{
}


GuiManager::~GuiManager()
{
	for (auto comp : components) {
		delete comp;
	}
}


void GuiManager::loadComponents(Renderer* renderer) {
	
	auto consolas64 = make_shared<GuiFont>(
		new Texture("Assets/Consolas64.png", 1024, 1024), 
		16, 64, 64, 36, 0);
	auto consolas32 = make_shared<GuiFont>(
		new Texture("Assets/Consolas32.png", 512, 512),
		16, 32, 32, 18, 0);
	auto consolas16 = make_shared<GuiFont>(
		new Texture("Assets/Consolas16.png", 256, 256),
		16, 16, 16, 9, 0);

	/* GUI Window Components */
	GuiComponent* background = new GuiComponent(0, 0, stoi(Settings[SETTING_WINDOW_WIDTH]), stoi(Settings[SETTING_WINDOW_HEIGHT]));
	background->setTexture(new Texture("Assets/background.png", 2560, 1600));
	background->resetPosition = [=]() { 
		background->setDimensions(0, 0, stoi(Settings[SETTING_WINDOW_WIDTH]), stoi(Settings[SETTING_WINDOW_HEIGHT]));
	};
	renderer->add(background);


	GuiText* fontTest = new GuiText(consolas64, 20, 20, 50, 50);
	fontTest->setText("Testing my font glyph renderer.\nThis is all being rendered from 1 string.");
	fontTest->resetPosition = [=]() {
		fontTest->setDimensions(20, 20, 50, 50);
	};
	fontTest->opacity = 0.9f;
	renderer->add(fontTest);

	GuiText* fontTest2 = new GuiText(consolas32, 20, 200, 50, 50);
	fontTest2->setText("Consolas 24pt, 32x32 test.\nSpacing seems a little odd though.");
	fontTest2->resetPosition = [=]() {
		fontTest2->setDimensions(20, 200, 50, 50);
	};
	fontTest2->opacity = 0.9f;
	renderer->add(fontTest2);

	GuiText* fpsDisplay = new GuiText(consolas16,
		10,
		stoi(Settings[SETTING_WINDOW_HEIGHT]) - 20,
		17, 17);
	fpsDisplay->opacity = 0.9f;
	fpsDisplay->resetPosition = [=]() {
		fpsDisplay->setDimensions(
			10,
			stoi(Settings[SETTING_WINDOW_HEIGHT]) - 20,
			17, 17);
	};
	fpsDisplay->addUpdateListener([=]() {
		string msg = "FPS: " + to_string(Time.fps);
		fpsDisplay->setText(msg);
	});
	renderer->add(fpsDisplay);


	GuiButton* quitButton = new GuiButton(
		stoi(Settings[SETTING_WINDOW_WIDTH]) - 27,
		10, 17, 17);
	quitButton->setTexture(new Texture("Assets/Close.png", 17, 17));
	quitButton->opacity = 0.5f;
	quitButton->resetPosition = [=]() { quitButton->setDimensions(
		stoi(Settings[SETTING_WINDOW_WIDTH]) - 27,
		10, 17, 17); };
	quitButton->addActionListener([](const ActionEvent& ae) {
		glfwSetWindowShouldClose(ae.window, 1);
	});
	quitButton->addMouseInListener([=](const MouseEvent& me) {
		quitButton->opacity = 0.8f;
	}); 
	quitButton->addMouseOutListener([=](const MouseEvent& me) {
		quitButton->opacity = 0.5f;
	});
	renderer->add(quitButton);

	GuiButton* maximizeButton = new GuiButton(
		stoi(Settings[SETTING_WINDOW_WIDTH]) - 47,
		10, 17, 17);
	maximizeButton->setTexture(new Texture("Assets/Maximize.png", 17, 17));
	maximizeButton->opacity = 0.5f;
	maximizeButton->resetPosition = [=]() { maximizeButton->setDimensions(
		stoi(Settings[SETTING_WINDOW_WIDTH]) - 47,
		10, 17, 17); };
	maximizeButton->addMouseInListener([=](const MouseEvent& me) {
		maximizeButton->opacity = 0.8f;
	});
	maximizeButton->addMouseOutListener([=](const MouseEvent& me) {
		maximizeButton->opacity = 0.5f;
	});
	renderer->add(maximizeButton);

	GuiButton* minimizeButton = new GuiButton(
		stoi(Settings[SETTING_WINDOW_WIDTH]) - 67,
		10, 17, 17);
	minimizeButton->setTexture(new Texture("Assets/Minimize.png", 17, 17));
	minimizeButton->opacity = 0.5f; 
	minimizeButton->resetPosition = [=]() { minimizeButton->setDimensions(
		stoi(Settings[SETTING_WINDOW_WIDTH]) - 67,
		10, 17, 17); };
	minimizeButton->addActionListener([](const ActionEvent& ae) {
		glfwIconifyWindow(ae.window);
	}); 
	minimizeButton->addMouseInListener([=](const MouseEvent& me) {
		minimizeButton->opacity = 0.8f;
	});
	minimizeButton->addMouseOutListener([=](const MouseEvent& me) {
		minimizeButton->opacity = 0.5f;
	});
	renderer->add(minimizeButton);

	GuiButton* restoreButton = new GuiButton(
		stoi(Settings[SETTING_WINDOW_WIDTH]) - 47,
		10, 17, 17);
	restoreButton->setTexture(new Texture("Assets/Windowed.png", 17, 17));
	restoreButton->opacity = 0.5f;
	restoreButton->resetPosition = [=]() { restoreButton->setDimensions(
		stoi(Settings[SETTING_WINDOW_WIDTH]) - 47,
		10, 17, 17); };	
	restoreButton->disable();
	restoreButton->hide();
	restoreButton->addMouseInListener([=](const MouseEvent& me) {
		restoreButton->opacity = 0.8f;
	});
	restoreButton->addMouseOutListener([=](const MouseEvent& me) {
		restoreButton->opacity = 0.5f;
	});
	renderer->add(restoreButton);

	GuiComponent* moveWidget = new GuiComponent(
		(stoi(Settings[SETTING_WINDOW_WIDTH]) - (stoi(Settings[SETTING_WINDOW_WIDTH]) - 160)) / 2, 0,
		(stoi(Settings[SETTING_WINDOW_WIDTH]) - 160), 100);
	moveWidget->resetPosition = [=]() { moveWidget->setDimensions(
		(stoi(Settings[SETTING_WINDOW_WIDTH]) - (stoi(Settings[SETTING_WINDOW_WIDTH]) - 160)) / 2, 0,
		(stoi(Settings[SETTING_WINDOW_WIDTH]) - 160), 100); };
	moveWidget->addMouseClickAndDragListener([=](const MouseEvent& me) {
		// move the window
		glfwSetWindowPos(me.window, 
			stoi(Settings[SETTING_WINDOW_POSITION_X]) + (int)me.mousePosition.x, 
			stoi(Settings[SETTING_WINDOW_POSITION_Y]) + (int)me.mousePosition.y);
		moveWidget->dragStart -= me.mousePosition;
	});

	GuiComponent* resizeWidget = new GuiComponent(
		stoi(Settings[SETTING_WINDOW_WIDTH]) - 19,
		stoi(Settings[SETTING_WINDOW_HEIGHT]) - 19,
		17, 17);
	resizeWidget->setTexture(new Texture("Assets/Resize.png", 17, 17));
	resizeWidget->opacity = 0.5f;
	resizeWidget->resetPosition = [=]() { resizeWidget->setDimensions(
		stoi(Settings[SETTING_WINDOW_WIDTH]) - 19,
		stoi(Settings[SETTING_WINDOW_HEIGHT]) - 19,
		17, 17); };
	resizeWidget->addMouseInListener([=](const MouseEvent& me) {
		resizeWidget->opacity = 0.8f;
	});
	resizeWidget->addMouseOutListener([=](const MouseEvent& me) {
		resizeWidget->opacity = 0.5f;
	});
	resizeWidget->addMouseClickAndDragListener([=](const MouseEvent& me) {
		// resize the window
		int width = max(stoi(Settings[SETTING_WINDOW_WIDTH]) + (int)me.mousePosition.x, MINIMUM_WINDOW_WIDTH);
		int height = max(stoi(Settings[SETTING_WINDOW_HEIGHT]) + (int)me.mousePosition.y, MINIMUM_WINDOW_HEIGHT);
		glfwSetWindowSize(me.window, width, height);
	});
	renderer->add(resizeWidget);


	maximizeButton->addActionListener([=](const ActionEvent& ae) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowPos(ae.window, 0, 0);
		glfwSetWindowSize(ae.window, mode->width, mode->height);

		maximizeButton->disable();
		maximizeButton->hide();

		restoreButton->enable();
		restoreButton->show();
	});

	restoreButton->addActionListener([=](const ActionEvent& ae) {
		// Reset to default window size and pos
		Settings[SETTING_WINDOW_WIDTH] = to_string(DEFAULT_WINDOW_WIDTH);
		Settings[SETTING_WINDOW_HEIGHT] = to_string(DEFAULT_WINDOW_HEIGHT);
		Settings[SETTING_WINDOW_POSITION_X] = to_string((glfwGetVideoMode(glfwGetPrimaryMonitor())->width - DEFAULT_WINDOW_WIDTH) / 2);
		Settings[SETTING_WINDOW_POSITION_Y] = to_string((glfwGetVideoMode(glfwGetPrimaryMonitor())->height - DEFAULT_WINDOW_HEIGHT) / 2);

		glfwSetWindowPos(ae.window, stoi(Settings[SETTING_WINDOW_POSITION_X]), stoi(Settings[SETTING_WINDOW_POSITION_Y]));
		glfwSetWindowSize(ae.window, stoi(Settings[SETTING_WINDOW_WIDTH]), stoi(Settings[SETTING_WINDOW_HEIGHT]));

		maximizeButton->enable();
		maximizeButton->show();

		restoreButton->disable();
		restoreButton->hide();
	});
}


void GuiManager::onWindowResize() 
{
	for (auto comp : components) {
		if (comp->resetPosition) {
			comp->resetPosition();
		}
	}
}


void GuiManager::onUpdate()
{
	for (auto comp : components) {
		comp->onUpdate();
	}
}