#pragma once

#include <chrono>
using namespace std::chrono;

#include "Settings.h"

class Timer
{
private:
	high_resolution_clock::time_point appStart;
	high_resolution_clock::time_point frameStart;
	high_resolution_clock::time_point frameEnd;

	float totalElapsedSinceLastSecond;
	int framesSinceLastSecond;
	float frameOffset;

	void Update();

public:
	float delta;
	float scale;
	float totalElapsed;

	int fps;

	float foregroundFPS;
	float backgroundFPS;

	bool hasFocus = true;

	Timer();
	void init();

	void waitForUpdate();
};

extern Timer Time;