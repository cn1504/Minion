#include "GuiText.h"


GuiText::GuiText(shared_ptr<GuiFont> font, GLuint x, GLuint y, GLuint width, GLuint height)
: GuiComponent(x, y, width, height)
{
	this->font = font;
}


GuiText::~GuiText()
{
}


void GuiText::setText(string msg) 
{
	txt = msg;
}


void GuiText::render(Shader* shader)
{
	// 'cursor' position
	GLfloat originalXPos = x;
	GLfloat originalYPos = y;

	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	for (auto& ch : txt) {

		// get ascii code as integer
		int ascii_code = ch;

		if (ascii_code == '\n') {
			y = font->nextLinePos(y);
			x = originalXPos;

			continue;
		}
		
		// work out row and column in atlas
		font->updateGlyphOffset(ascii_code);

		// work out position of glyph
		setDimensions((GLint)x, (GLint)y, (GLint)(font->getCharacterWidth()), (GLint)(font->getCharacterHeight()));

		GuiComponent::render(shader);

		x = font->nextCharPos(x);
	}

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	x = originalXPos;
	y = originalYPos;
}


void GuiFont::updateGlyphOffset(int ascii)
{
	int atlas_col = (ascii - ' ') % charPerRow;
	int atlas_row = (ascii - ' ') / charPerRow;

	// work out texture coordinates in atlas
	font->setOffset(atlas_col * (1.0f / ((float)charPerRow)),
		atlas_row * (1.0f / ((float)charPerRow)));
};