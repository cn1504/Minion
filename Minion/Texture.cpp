#include "Texture.h"


Texture::Texture(const string& pngFilename, GLuint width, GLuint height, 
	GLfloat offsetS, GLfloat offsetT, GLfloat spriteWidth, GLfloat spriteHeight)
{	
	vector<unsigned char> image;

	unsigned error = lodepng::decode(image, width, height, pngFilename);

	// If there's an error, display it.
	if (error != 0)
	{
		string msg = lodepng_error_text(error);
		ExitOnError(("lodepng error: " + msg).c_str());
	}

	// Create one OpenGL texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	

	// Add Anisotropic Filtering
	GLfloat maxAniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);

	// ... nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

	this->width = width;
	this->height = height;
	filename = pngFilename;

	s = offsetS;
	t = offsetT;
	this->spriteWidth = spriteWidth;
	this->spriteHeight = spriteHeight;
}


Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}
