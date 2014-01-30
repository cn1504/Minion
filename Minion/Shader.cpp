#include "Shader.h"


Shader::Shader(string vertex, string fragment)
{
	vector<GLuint> shaderList;

	shaderList.push_back(createShader(GL_VERTEX_SHADER, vertex));
	shaderList.push_back(createShader(GL_FRAGMENT_SHADER, fragment));

	programId = createProgram(shaderList);

	for(auto shader : shaderList)
		glDeleteShader(shader);
}


Shader::Shader(string vertex, string tess_control, string tess_eval, string fragment)
{
	vector<GLuint> shaderList;

	shaderList.push_back(createShader(GL_VERTEX_SHADER, vertex));
	shaderList.push_back(createShader(GL_TESS_CONTROL_SHADER, tess_control));
	shaderList.push_back(createShader(GL_TESS_EVALUATION_SHADER, tess_eval));
	shaderList.push_back(createShader(GL_FRAGMENT_SHADER, fragment));

	programId = createProgram(shaderList);

	for (auto shader : shaderList)
		glDeleteShader(shader);
}


Shader::~Shader()
{
	glDeleteProgram(programId);
}


GLuint Shader::createShader(GLenum eShaderType, const string &strShaderFile)
{
	string shaderFilepath(SHADER_PATH + strShaderFile);
	ifstream ifs(shaderFilepath);
	if (!ifs.is_open()) {
		ExitOnError(("Could not open: " + shaderFilepath).c_str());
	}
	string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
	ifs.close();

	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = str.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_TESS_CONTROL_SHADER: strShaderType = "tessellation control"; break;
		case GL_TESS_EVALUATION_SHADER: strShaderType = "tessellation evaluation"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader %s:\n%s\n", strShaderType, strShaderFile, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}


GLuint Shader::createProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for (auto shader : shaderList)
		glAttachShader(program, shader);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}


void Shader::use()
{
	glUseProgram(programId);
}