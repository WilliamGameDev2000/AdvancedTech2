#pragma once

#include "Window.h"
#include "Cube.h"
#include <chrono>

using namespace std::chrono;

class Game
{
public:
	Game();
	~Game();

	int Go();

private:
	void Update();
	float Mark();

	Window wnd;
	std::chrono::steady_clock::time_point last;
	float rotation;
	const static int numCubes = 80;
	std::vector<std::unique_ptr<class Cube>> Cubes;
};
