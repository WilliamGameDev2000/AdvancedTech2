#include "Game.h"

Game::Game() : wnd(1024, 720, 100, 50, "AT GAME WINDOW")
{}

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
	wnd.Gfx().DrawTriangle();
	wnd.Gfx().EndFrame();
}