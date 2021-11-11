#include "Game.h"
#include "Cube.h"
#include <memory>
#include <fstream>

Game::Game() : wnd(1024, 720, 0, 0, "AT GAME WINDOW")
{
	last = steady_clock::now();

	LoadLevel("MapFile");

	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	wnd.Gfx().SetCamera(DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f));
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
	auto dt = Mark();

	if (wnd.KB.KeyIsPressed('A'))
	{
		wnd.Gfx().cam.Translate(-speed * dt, 0.0f, 0.0f);
	}
	else if (wnd.KB.KeyIsPressed('D'))
	{
		wnd.Gfx().cam.Translate(speed * dt, 0.0f, 0.0f);
	}
	else if (wnd.KB.KeyIsPressed('W'))
	{
		wnd.Gfx().cam.Translate(0.0f, 0.0f, speed * dt);
	}
	else if (wnd.KB.KeyIsPressed('Q'))
	{
		wnd.Gfx().cam.Rotate(-speed * dt,0.0f, 0.0f );
	}
	else if (wnd.KB.KeyIsPressed('E'))
	{
		wnd.Gfx().cam.Rotate(speed * dt, 0.0f  , 0.0f);
	}
	else if (wnd.KB.KeyIsPressed('O'))
	{
		wnd.Gfx().cam.Rotate(0.0f, speed * dt, 0.0f);
	}
	else if (wnd.KB.KeyIsPressed('P'))
	{
		wnd.Gfx().cam.Rotate(0.0f, -speed * dt, 0.0f);
	}

	////REMOVE MOUSE STUFF, NOT NECESSARY
	//while (!wnd.mouse.IsEmpty())
	//{
	//	
	//	const auto e = wnd.mouse.Read();
	//	if (e.GetType() == Mouse::Event::Type::Move)
	//	{
	//		//changes camera direction based on movement in each quadrent with this system
	//		wnd.Gfx().cam.SetYaw(((((float)e.GetPosX() / 1024) * 2) - 1));
	//		wnd.Gfx().cam.SetPitch(((((float)e.GetPosY() / 720) * 2) - 1));
	//	}
	//	
	//}
	wnd.Gfx().cam.UpdateCamera();	
	wnd.Gfx().ClearBuffer(0.7f, 0.0f, 0.12f);
	

	for (auto& c : Cubes)
	{
		//c->Update(dt);
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

void Game::LoadLevel(std::string level_file)
{
	LoadWall(level_file);
	LoadFloor(level_file);
}

void Game::LoadWall(std::string level_file)
{
	std::ifstream levelFile;
	levelFile.open(level_file + ".txt");
	while (!levelFile.eof())
	{
		if (levelFile.peek() == '\n')
		{
			column = 0;
			row++;
		}

		if (levelFile.get() == wallBlock)
		{
			Cubes.push_back(std::make_unique<Cube>(wnd.Gfx()));
			Cubes[numCubes]->setPos((column * 2) - 11, 0, (row * 2) - 6);
			numCubes++;

		}

		column++;
	}
	levelFile.close();
}

void Game::LoadFloor(std::string level_file)
{
	column = 0;
	row = 0;
	std::ifstream levelFile;
	levelFile.open(level_file + ".txt");
	while (!levelFile.eof())
	{
		if (levelFile.peek() == '\n')
		{
			column = 0;
			row++;
		}

		if (levelFile.get() == floorBlock)
		{
			Cubes.push_back(std::make_unique<Cube>(wnd.Gfx()));
			Cubes[numCubes]->setPos((column * 2) - 11, -2, (row * 2) - 6);
			numCubes++;

		}

		column++;
	}
	levelFile.close();
}


