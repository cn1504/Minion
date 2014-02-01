#include "GuiComponent.h"
#include "InputHandler.h"
#include "GuiManager.h"


GuiComponent::GuiComponent(GLuint x, GLuint y, GLuint width, GLuint height)
{
	glGenVertexArrays(1, &quad_VertexArrayID);

	quad_vertexbuffer = 0;
	glGenBuffers(1, &quad_vertexbuffer);

	setDimensions(x, y, width, height);
	opacity = 1.0f;
	hidden = false;
	mouseOver = false;
	dragged = false;

	Input.addGuiComponent(this);
	GUI.addComponent(this);

	resetPosition = [](){};
}


void GuiComponent::render(Shader* shader)
{
	if (hidden)
		return;

	glUniform1f(glGetUniformLocation(shader->programId, SU_GUI_OPACITY), opacity);
	glUniform2f(glGetUniformLocation(shader->programId, SU_GUI_TEXTURE_OFFSET), getTexture()->getOffsetS(), getTexture()->getOffsetT());
	glUniform2f(glGetUniformLocation(shader->programId, SU_GUI_TEXTURE_DIMENSIONS), getTexture()->getSpriteWidth(), getTexture()->getSpriteHeight());

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer(
		0,                                // attribute
		4,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}


void GuiComponent::setTexture(Texture* texture) 
{
	unique_ptr<Texture> p(texture);
	this->texture = move(p);
}


void GuiComponent::setDimensions(GLuint left, GLuint top, GLuint sizeWidth, GLuint sizeHeight)
{
	// Convert from pixel counts to opengl coords
	GLfloat wWidth = stof(Settings[SETTING_WINDOW_WIDTH]);
	GLfloat wHeight = stof(Settings[SETTING_WINDOW_HEIGHT]);
	GLfloat wHalfWidth = wWidth / 2.0f;
	GLfloat wHalfHeight = wHeight / 2.0f;

	// Store screen coords
	// cout << "Setting size to (" << left << ", " << top << ", " << sizeWidth << ", " << sizeHeight << ")" << endl;
	x = (float)left;
	y = (float)top;
	width = (float)sizeWidth;
	height = (float)sizeHeight;

	vec2 pos((x - wHalfWidth) / wHalfWidth, (wHeight - y - wHalfHeight) / wHalfHeight);
	//GLfloat finalX = x / wWidth;
	//GLfloat finalY = y / wHeight;
	vec2 dim(width / wHalfWidth, height / wHalfHeight);
	
	GLfloat g_quad_vertex_buffer_data[] = {
		pos.x, pos.y - dim.y, 0.0f, 1.0f,
		pos.x + dim.x, pos.y - dim.y, 1.0f, 1.0f,
		pos.x, pos.y, 0.0f, 0.0f,
		pos.x, pos.y, 0.0f, 0.0f,
		pos.x + dim.x, pos.y - dim.y, 1.0f, 1.0f,
		pos.x + dim.x, pos.y, 1.0f, 0.0f
	};
	
	// Update gpu memory
	glBindVertexArray(quad_VertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_DYNAMIC_DRAW);
}

void GuiComponent::onMouseIn(MouseEvent& me)
{
	if (hidden)
		return;

	for (auto listener : mouseInListeners) {
		listener(me);
	}
}

void GuiComponent::onMouseOut(MouseEvent& me)
{
	if (hidden)
		return;

	for (auto listener : mouseOutListeners) {
		listener(me);
	}
}

void GuiComponent::onMouseClickAndDrag(MouseEvent& me)
{
	if (dragged) {
		// Continue Drag
		vec2 currentDelta = me.mousePosition - dragStart;

		if (currentDelta == prevDragDelta) {
			// Ignore drag event
		}
		else {
			me.mousePosition = currentDelta - prevDragDelta;
			prevDragDelta = currentDelta;

			for (auto listener : mouseClickAndDragListeners) {
				listener(me);
			}
		}
	}
	else {
		// Drag starting
		dragStart = me.mousePosition;
		prevDragDelta = vec2(0, 0);
		dragged = true;
	}
}

void GuiComponent::onMouseClickRelease(MouseEvent& me)
{
	dragged = false;
}

void GuiComponent::onUpdate()
{
	if (hidden)
		return;

	for (auto listener : updateListeners) {
		listener();
	}
}