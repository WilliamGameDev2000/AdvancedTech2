#pragma once

#include "Window.h"
#include <chrono>

using namespace std::chrono;

class Game
{
public:
	Game();

	int Go();

private:
	void Update();

	Window wnd;
	std::chrono::steady_clock::time_point last;
	float rotation;
	/*std::pair<float, float> Point1 = { 0.0F, 0.5F };
	std::pair<float, float> Point2 = { 0.5F, -0.5F };
	std::pair<float, float> Point3 = { -0.5F, -0.5F };*/
};
