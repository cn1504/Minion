#include "Time.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <string>

typedef std::chrono::duration<double> dsec;
Timer Time;

Timer::Timer()
{
	appStart = chrono::high_resolution_clock::now();
	totalElapsed = 0.0f;
	scale = 1.0f;

	totalElapsedSinceLastSecond = 0.0f;
	framesSinceLastSecond = 0;
	fps = 0;
	frameOffset = 1.5;
}

void Timer::init()
{
	/* Make sure MaxForegroundFPS and MaxBackgroundFPS are set */
	foregroundFPS = stof(Settings.assertValue("MaxForegroundFPS", to_string(DEFAULT_MAX_FOREGROUND_FPS)));
	backgroundFPS = stof(Settings.assertValue("MaxBackgroundFPS", to_string(DEFAULT_MAX_BACKGROUND_FPS)));

	frameStart = frameEnd = high_resolution_clock::now();
}

void Timer::waitForUpdate() 
{
	// Sleep if necessary
	if (std::chrono::high_resolution_clock::now() < frameEnd) {
		this_thread::sleep_until(frameEnd);
	}

	// Get start time
	auto prevStart = frameStart;
	frameStart = high_resolution_clock::now();

	// Set public member data
	delta = (float)(duration_cast<dsec>(frameStart - prevStart).count() * scale);
	totalElapsed += delta;
	
	// Calculate end of frame
	dsec frameDuration(1.0 / (double)((hasFocus) ? foregroundFPS : backgroundFPS));
	frameEnd = frameEnd + duration_cast<high_resolution_clock::duration>(frameDuration);

	// Calculate Frames per second
	framesSinceLastSecond++;
	totalElapsedSinceLastSecond += delta;

	if (totalElapsedSinceLastSecond > 1.0f) {
		fps = framesSinceLastSecond;
		framesSinceLastSecond = 0;
		totalElapsedSinceLastSecond = 0.0f;

		// Sync the sleep timer
		frameEnd = frameStart + duration_cast<high_resolution_clock::duration>(frameDuration);
	}
}