#pragma once

#include "StdInc.h"

#include <LoadPNG/lodepng.h>

class Texture
{
private:
	GLuint textureID;

	string filename;
	GLuint width, height;

public:
	Texture(const string& pngFilename, GLuint width, GLuint height);
	~Texture();

	GLuint getID()	   { return textureID; }
	GLuint getWidth()  { return width; }
	GLuint getHeight() { return height; }
};

