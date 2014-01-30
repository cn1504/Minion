#pragma once

#include "StdInc.h"
#include "Shader.h"
#include "GuiComponent.h"

class Renderer
{
public:
	Renderer();
	~Renderer(void);

	bool isMinimized;

	void resizeWindow(int windowWidth, int windowHeight);

	void renderScene();

	void add(GuiComponent* component);

protected:
	void renderGUI();
	void renderToScreen();

	void drawScreenQuad();

	void generateScreenTexture(GLuint & into, bool depth = false, int width = 0, int height = 0);

	// Frame buffer objects
	GLuint screenOutputFBO;
	GLuint screenOutputTexture;

	GLuint guiFBO;
	GLuint guiTexture;

	int width, height;

	// Screen output quad
	GLuint quad_vertexbuffer;

	// Shader programs
	unique_ptr<Shader> ppShader;	// Post processing shader
	unique_ptr<Shader> bcShader;	// Buffer combining shader
	unique_ptr<Shader> guiShader;	// Gui rendering shader


	// Renderable object collections
	vector<GuiComponent*> guiComponents;
};

extern Renderer* renderer;

