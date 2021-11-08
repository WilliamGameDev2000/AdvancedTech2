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

	int Init();

private:
	void Update();
	float Mark();

	Window wnd;
	std::chrono::steady_clock::time_point last;
	float rotation;
	int numCubes = 0;
	int row = 0;
	int column = 0;
	char wallBlock = '#';
	std::vector<std::unique_ptr<class Cube>> Cubes;
};
