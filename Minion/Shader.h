#pragma once

#include "StdInc.h"

class Shader
{
private:
	GLuint createShader(GLenum eShaderType, const std::string &strShaderFile);
	GLuint createProgram(const vector<GLuint> &shaderList);

public:
	Shader(string vertex, string fragment);
	Shader(string vertex, string tess_control, string tess_eval, string fragment);
	~Shader();

	GLuint programId;

	void use();
};

