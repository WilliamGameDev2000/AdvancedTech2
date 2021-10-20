#pragma once

#include "Window.h"

class Game
{
public:
	Game();

	int Go();

private:
	void Update();

	Window wnd;
};
