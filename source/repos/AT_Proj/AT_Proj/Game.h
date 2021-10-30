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
};
