
// External includes
#include "Constants.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <memory>
#include <functional>
using namespace std;

#define GLEW_STATIC
#include <GL/glew.h>
#pragma comment(lib, "opengl32.lib")
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;

#include "Time.h"

void ExitOnGLError(const char* error_message);
void ExitOnError(const char* error_message);