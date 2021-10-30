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

	

	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		if (e.GetType() == Mouse::Event::Type::Move)
		{
			std::ostringstream oss;
			oss << "Mouse PosX: (" << e.GetPosX() << ", " << e.GetPosY() << ")";
			wnd.SetTitle(oss.str());
		}
	}
	rotation = duration<float>(steady_clock::now() - last).count();

	const float c = sin(rotation) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, c);
	wnd.Gfx().DrawCube(-1 * rotation, 0.0f,
		0.0f);
	wnd.Gfx().DrawCube(rotation, wnd.mouse.GetPosX() / 512.0f - 1.0f,
		-wnd.mouse.GetPosY() / 360.0f + 1.0f);
	wnd.Gfx().EndFrame();
}