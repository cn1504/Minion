
// Includes
//------------------------------------------------------------------------------------
#include "StdInc.h"
#include "Settings.h"
#include "Renderer.h"
#include "Time.h"
#include "Shader.h"
#include "InputHandler.h"
#include "GuiManager.h"

// callbacks and error handlers
//------------------------------------------------------------------------------------
void error_callback(int error, const char* description)
{
	ExitOnError(description);
}

void ExitOnGLError(const char* error_message)
{
	const GLenum ErrorValue = glGetError();

	if (ErrorValue != GL_NO_ERROR) {
		string msg(error_message);
		ExitOnError((msg + " " + to_string(ErrorValue)).c_str());
	}
}

void ExitOnError(const char* error_message)
{
	if (DEBUG_MODE) {
		cout << endl << error_message << endl;
		system("PAUSE");
	}

	std::ofstream fout(CRASH_LOG_FILE);
	if (fout.is_open()) {
		fout << error_message << std::endl;
		fout.close();
	}

	delete renderer;
	glfwTerminate();
	exit(EXIT_FAILURE);
}

void windowFocus_callback(GLFWwindow* window, int state)
{
	Time.hasFocus = (state == GL_TRUE);
}

void mouseButton_callback(GLFWwindow* window, int button, int action, int mods) {
	if (!glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		return;
	}

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
		Input.onMouseLeftClick(window, vec2((float)mouseX, (float)mouseY));
	}
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
		Input.onMouseLeftDown(window, vec2((float)mouseX, (float)mouseY));
	}
}

void windowResize_callback(GLFWwindow* window, int width, int height) {
	if (width < 1)
		return;

	Settings[SETTING_WINDOW_WIDTH] = to_string(width);
	Settings[SETTING_WINDOW_HEIGHT] = to_string(height);

	renderer->resizeWindow(stoi(Settings[SETTING_WINDOW_WIDTH]), stoi(Settings[SETTING_WINDOW_HEIGHT]));
	GUI.onWindowResize();
}

void windowIconified_callback(GLFWwindow* window, int state) {
	if (state == GL_TRUE) {		// Minimized
		renderer->isMinimized = true;
	}
	else {						// Restored
		renderer->isMinimized = false;
	}
}

void windowMove_callback(GLFWwindow* window, int x, int y) {
	Settings[SETTING_WINDOW_POSITION_X] = to_string(x);
	Settings[SETTING_WINDOW_POSITION_Y] = to_string(y);
}

// main method - program entry point
//------------------------------------------------------------------------------------
int main(void)
{
	if (DEBUG_MODE) {
		cout << "Debug Mode Active" << endl << endl;
	}

	Settings.load(SETTINGS_FILE);

	/* Initialize the library */
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		return -1;

	if (DEBUG_MODE) {

		/* Get display information */
		int monitorCount;
		GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

		/* Output display information */
		cout << "Available Monitors:" << endl << endl;
		for (int i = 0; i < monitorCount; i++) {
			cout << "\t" << i+1 << ". " << glfwGetMonitorName(monitors[i]);
			const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);
			cout << " - " << mode->width << "x" << mode->height << "x" << mode->redBits * 4 << " @ " << mode->refreshRate << "hz" << endl;
		}

		cout << endl;
		system("PAUSE");

	}

	/* Create a windowed mode window and its OpenGL context */
	glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(
		stoi(Settings.assertValue(SETTING_WINDOW_WIDTH, to_string(DEFAULT_WINDOW_WIDTH))),
		stoi(Settings.assertValue(SETTING_WINDOW_HEIGHT, to_string(DEFAULT_WINDOW_HEIGHT))),
		"Minion", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetWindowPos(window,
		stoi(Settings.assertValue(SETTING_WINDOW_POSITION_X, to_string((glfwGetVideoMode(glfwGetPrimaryMonitor())->width - DEFAULT_WINDOW_WIDTH) / 2))),
		stoi(Settings.assertValue(SETTING_WINDOW_POSITION_Y, to_string((glfwGetVideoMode(glfwGetPrimaryMonitor())->height - DEFAULT_WINDOW_HEIGHT) / 2))));
	glfwSetWindowFocusCallback(window, windowFocus_callback);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Initialize Glew
	if (GLEW_OK != glewInit()) {
		error_callback(0, "Glew Failed to Initialize");
	}
	glGetError();

	// Initialize rendering window
	renderer = new Renderer();
	renderer->resizeWindow(stoi(Settings[SETTING_WINDOW_WIDTH]), stoi(Settings[SETTING_WINDOW_HEIGHT]));
	glfwSetWindowIconifyCallback(window, windowIconified_callback);

	/* Initialize Timer */
	Time.init();
	
	GUI.loadComponents(renderer);
	glfwSetMouseButtonCallback(window, mouseButton_callback);
	glfwSetWindowSizeCallback(window, windowResize_callback);
	glfwSetWindowPosCallback(window, windowMove_callback);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		/* Render here */
		renderer->renderScene();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		Time.waitForUpdate();

		/* Poll for and process events */
		glfwPollEvents();

		double x, y;
		glfwGetCursorPos(window, &x, &y);
		Input.onUpdate(window, vec2((float)x, (float)y));

		GUI.onUpdate();
	}

	Settings.save();
	delete renderer;
	glfwTerminate();
	return 0;
}