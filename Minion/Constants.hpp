#pragma once

#define DEBUG_MODE 1

#if DEBUG_MODE == 0
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")	// Hides the console window
#endif

#define SETTINGS_FILE "settings.ini"
#define CRASH_LOG_FILE "crashlog.txt"
#define SHADER_PATH "Shaders/"
#define ASSETS_PATH "Assets/"

#define SETTING_WINDOW_WIDTH "WindowWidth"
#define DEFAULT_WINDOW_WIDTH 1280
#define MINIMUM_WINDOW_WIDTH 600
#define SETTING_WINDOW_HEIGHT "WindowHeight"
#define DEFAULT_WINDOW_HEIGHT 720
#define MINIMUM_WINDOW_HEIGHT 400

// Window position - Defaults to center of primary screen
#define SETTING_WINDOW_POSITION_X "WindowPosX"
#define SETTING_WINDOW_POSITION_Y "WindowPosY"

#define SETTING_MAX_FOREGROUND_FPS "MaxForegroundFPS"
#define DEFAULT_MAX_FOREGROUND_FPS 120
#define SETTING_MAX_BACKGROUND_FPS "MaxBackgroundFPS"
#define DEFAULT_MAX_BACKGROUND_FPS 15


#define SHADER_POSTPROCESSING_VERT_FILE "fspassthrough.vert"
#define SHADER_POSTPROCESSING_FRAG_FILE "fxaa.frag"
#define SU_PP_SOURCE_TEXTURE "sourceTexture"
#define SU_PP_FRAME_SIZE "frameSize"
#define SU_PP_GUI_TEXTURE "guiTexture"

#define SHADER_BUFFER_COMBINE_VERT_FILE "fspassthrough.vert"
#define SHADER_BUFFER_COMBINE_FRAG_FILE "combinebuffers.frag"
#define SU_BC_DIFFUSE_TEXTURE "diffuseTex"

#define SHADER_GUI_VERT_FILE "gui.vert"
#define SHADER_GUI_FRAG_FILE "gui.frag"
#define SU_GUI_TEXTURE "compTexture"
#define SU_GUI_OPACITY "opacity"