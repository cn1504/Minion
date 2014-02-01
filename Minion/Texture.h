#pragma once

#include "StdInc.h"

#include <LoadPNG/lodepng.h>

class Texture
{
private:
	GLuint textureID;

	string filename;
	GLuint width, height;
	GLfloat s, t;
	GLfloat spriteWidth, spriteHeight;

public:
	Texture(const string& pngFilename, GLuint width, GLuint height, 
		GLfloat offsetS = 0, GLfloat offsetT = 0, 
		GLfloat spriteWidth = 1.0f, GLfloat spriteHeight = 1.0f);
	~Texture();

	GLuint getID()	   { return textureID; }
	GLuint getWidth()  { return width; }
	GLuint getHeight() { return height; }
	GLfloat getOffsetS() { return s; }
	GLfloat getOffsetT() { return t; }
	GLfloat getSpriteWidth() { return spriteWidth; }
	GLfloat getSpriteHeight() { return spriteHeight; }

	void setOffset(float s, float t) { this->s = s, this->t = t; }
	void setSpriteDimensions(float w, float h) { spriteWidth = w, spriteHeight = h; }
};

