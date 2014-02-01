#pragma once
#include "StdInc.h"
#include "Renderable.h"
#include "Texture.h"

class MouseEvent {
public:
	MouseEvent(GLFWwindow* window, vec2 mouse) { this->window = window, mousePosition = mouse; }
	virtual ~MouseEvent() {};

	vec2 mousePosition;
	GLFWwindow* window;
};

class GuiComponent : public Renderable
{
protected:
	vector<function<void()>> updateListeners;
	vector<function<void(const MouseEvent&)>> mouseInListeners;
	vector<function<void(const MouseEvent&)>> mouseOutListeners;
	vector<function<void(const MouseEvent&)>> mouseClickAndDragListeners;

	unique_ptr<Texture> texture;

	// Dimensions
	float x, y;
	float width, height;
	
	// Geometry
	GLuint quad_vertexbuffer;
	GLuint quad_VertexArrayID;
	// GLfloat g_quad_vertex_buffer_data[24];	// 6 * vec4

	bool hidden;

public:
	GuiComponent(GLuint x, GLuint y, GLuint width, GLuint height);
	virtual ~GuiComponent() {};

	virtual Texture* getTexture() { return &(*texture); }
	void setTexture(Texture* texture);
	void setDimensions(GLuint x, GLuint y, GLuint width, GLuint height);

	function<void()> resetPosition;

	virtual void render(Shader* shader);

	float opacity;
	vec2 getPosition() { return vec2(x, y); }
	vec2 getDimensions() { return vec2(width, height); }

	void hide() { hidden = true; }
	void show() { hidden = false; }

	void addUpdateListener(function<void()> func) { updateListeners.push_back(func); }
	void addMouseInListener(function<void(const MouseEvent&)> func) { mouseInListeners.push_back(func); }
	void addMouseOutListener(function<void(const MouseEvent&)> func) { mouseOutListeners.push_back(func); }
	void addMouseClickAndDragListener(function<void(const MouseEvent&)> func) { mouseClickAndDragListeners.push_back(func); }

	bool mouseOver;
	void onMouseIn(MouseEvent& me);
	void onMouseOut(MouseEvent& me);

	bool dragged;
	vec2 dragStart;
	vec2 prevDragDelta;
	void onMouseClickAndDrag(MouseEvent& me);
	void onMouseClickRelease(MouseEvent& me);

	void onUpdate();
};

