#include "Game.h"
#include "Cube.h"
#include <memory>
#include <fstream>

Game::Game() : wnd(1024, 720, 0, 0, "AT GAME WINDOW")
{
	last = steady_clock::now();

	std::fstream levelFile;
	levelFile.open("MapFile.txt");
	while (!levelFile.eof())
	{
		if (row == 9)
		{
			row = 0;
			column++;
		}

		if (levelFile.get() == wallBlock)
		{
			Cubes.push_back(std::make_unique<Cube>(wnd.Gfx()));
			Cubes[numCubes]->setPos((row * 2) - 4.5, (column * 2) - 4.5, 10);
			numCubes++;

		}
		row++;
	}
	levelFile.close();
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

int Game::Init()
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

	while (!wnd.mouse.IsEmpty())
	{
		
		const auto e = wnd.mouse.Read();
		if (e.GetType() == Mouse::Event::Type::Move)
		{
			wnd.Gfx().cam.SetYaw(wnd.mouse.GetPosX());
			wnd.Gfx().cam.SetPitch(wnd.mouse.GetPosY());
					
		}
		wnd.Gfx().cam.UpdateCamera();
	}

	wnd.Gfx().ClearBuffer(0.7f, 0.0f, 0.12f);
	auto dt = Mark();

	for (auto& c : Cubes)
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