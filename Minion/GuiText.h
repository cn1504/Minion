#pragma once
#include "GuiComponent.h"

class GuiFont 
{
private:
	unique_ptr<Texture> font;

	GLuint charPerRow;
	GLuint charWidth;
	GLuint charHeight;
	GLuint widthStride;
	GLuint heightStride;

public:
	GuiFont(Texture* font, GLuint charPerRow, GLuint charWidth, GLuint charHeight, GLuint widthStride = 0, GLuint heightStride = 0)
		: font(font), charPerRow(charPerRow), charWidth(charWidth), charHeight(charHeight), widthStride(widthStride), heightStride(heightStride) {
		font->setSpriteDimensions(((GLfloat)charWidth) / ((GLfloat)font->getWidth()), ((GLfloat)charHeight) / ((GLfloat)font->getHeight()));
	}
	~GuiFont() { font = nullptr; };

	void updateGlyphOffset(int ascii);

	Texture* getTexture() { return &(*font); }
	GLfloat nextCharPos(GLfloat x, GLfloat scale = 1.0f) { return x + (charWidth - widthStride) * scale; }
	GLfloat nextLinePos(GLfloat y, GLfloat scale = 1.0f) { return y + (charHeight - heightStride) * scale; }
	GLuint getCharacterWidth() { return charWidth; }
	GLuint getCharacterHeight() { return charHeight; }
};

class GuiText :
	public GuiComponent
{
protected:
	shared_ptr<GuiFont> font; 
	string txt;

public:
	GuiText(shared_ptr<GuiFont> font, GLuint x, GLuint y, GLuint width, GLuint height);
	virtual ~GuiText();

	void setText(string msg);

	virtual void render(Shader* shader);
	virtual Texture* getTexture() { return font->getTexture(); }
};

