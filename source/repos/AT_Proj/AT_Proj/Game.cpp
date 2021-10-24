#include "Game.h"

Game::Game() : wnd(1024, 720, 0, 0, "AT GAME WINDOW")
{
	last = steady_clock::now();
}

int Game::Go()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessge())
		{
			return *ecode;
		}
		Update();
	}
		
}

void Game::Update()
{

	rotation = duration<float>(steady_clock::now() - last).count();

	const float c = sin(rotation) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().DrawTriangle(rotation);
	wnd.Gfx().EndFrame();
}