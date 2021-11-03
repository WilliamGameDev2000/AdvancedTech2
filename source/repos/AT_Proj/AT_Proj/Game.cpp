#include "Game.h"
#include "Cube.h"
#include <memory>

Game::Game() : wnd(1024, 720, 0, 0, "AT GAME WINDOW")
{
	last = steady_clock::now();
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < numCubes; i++)
	{
		Cubes.push_back(std::make_unique<Cube>(
			wnd.Gfx(), rng, adist,
			ddist, odist, rdist
			));
	}
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
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

Game::~Game()
{}

void Game::Update()
{

	

	/*while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		if (e.GetType() == Mouse::Event::Type::Move)
		{
			std::ostringstream oss;
			oss << "Mouse PosX: (" << e.GetPosX() << ", " << e.GetPosY() << ")";
			wnd.SetTitle(oss.str());
		}
	}*/

	wnd.Gfx().ClearBuffer(0.7f, 0.0f, 0.12f);
	auto dt = Mark();
	for(auto& c : Cubes)
	{
		c->Update(dt);
		c->Draw(wnd.Gfx());
	}
	wnd.Gfx().EndFrame();
}

float Game::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}