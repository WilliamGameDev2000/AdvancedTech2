#include "Game.h"
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
		wnd.Gfx().cam.Translate(-speed * dt, 0.0f);
		traveling = left;
	}
	else if (wnd.KB.KeyIsPressed('D'))
	{
		wnd.Gfx().cam.Translate(speed * dt, 0.0f);
		traveling = right;
	}
	if (wnd.KB.KeyIsPressed('W'))
	{
		wnd.Gfx().cam.Translate(0.0f, speed * dt);
		traveling = forward;
	}
	else if (wnd.KB.KeyIsPressed('S'))
	{
		wnd.Gfx().cam.Translate(0.0f, -speed * dt);
		traveling = back;
	}
	if (wnd.KB.KeyIsPressed('Q'))
	{
		wnd.Gfx().cam.Rotate(-speed * dt, 0.0f);
	}
	else if (wnd.KB.KeyIsPressed('E'))
	{
		wnd.Gfx().cam.Rotate(speed * dt, 0.0f);
	}
	if (wnd.KB.KeyIsPressed('O'))
	{
		wnd.Gfx().cam.Rotate(0.0f, speed * dt);
	}
	else if (wnd.KB.KeyIsPressed('P'))
	{
		wnd.Gfx().cam.Rotate(0.0f, -speed * dt);
	}
	if (wnd.KB.KeyIsPressed(' '))
	{
		Bullets.push_back(std::make_unique<Cube>(wnd.Gfx()));
		Bullets[numBullets]->setBullet();
		Bullets[numBullets]->setRot(wnd.Gfx().cam.getYaw());
		Bullets[numBullets]->setScale(0.2f, 0.2f, 0.2f);
		Bullets[numBullets]->setPos(wnd.Gfx().cam.getPos().x + sin(wnd.Gfx().cam.getYaw()), 0, wnd.Gfx().cam.getPos().z + cos(wnd.Gfx().cam.getYaw()));
		numBullets++;
		
	}


	wnd.Gfx().ClearBuffer(0.7f, 0.0f, 0.12f);
	for (auto& c : Cubes)
	{
		if (c->isColliding(wnd.Gfx().cam.getPos(), 0.5, 0.5, 0.5))
		{
			if (wnd.Gfx().cam.getForwardBack() != 0 || wnd.Gfx().cam.getLeftRight() != 0)
			{
				wnd.Gfx().cam.Translate(-1 * wnd.Gfx().cam.getLeftRight(), -1 * wnd.Gfx().cam.getForwardBack());
			}
			else
			{
				if (traveling == forward)
				{
					wnd.Gfx().cam.Translate(0.0f, wnd.Gfx().cam.getForwardBack() - 0.1);
				}
				if (traveling == back)
				{
					wnd.Gfx().cam.Translate(0.0f, wnd.Gfx().cam.getForwardBack() + 0.1);
				}
				if (traveling == left)
				{
					wnd.Gfx().cam.Translate(wnd.Gfx().cam.getLeftRight() + 0.1, 0.0f);
				}
				if (traveling == right)
				{
					wnd.Gfx().cam.Translate(wnd.Gfx().cam.getLeftRight() - 0.1, 0.0f);
				}	
				traveling = stationary;
			}
			
		}
		c->Draw(wnd.Gfx());
	}
	for (auto& e : Enemies)
	{
		e->Draw(wnd.Gfx());
		//e->Update(dt);
	}
	for (auto& B : Bullets)
	{
		//if(shot)
		B->Draw(wnd.Gfx());
		B->Update(dt);
		/*if (B->isColliding(wnd.Gfx().cam.getPos(), 0.5, 0.5, 0.5))
		{
			OutputDebugString("Shot");
		}*/
	}
	
	
	wnd.Gfx().cam.UpdateCamera();
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
	LoadEnemies(level_file);
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

void Game::LoadEnemies(std::string level_file)
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

		if (levelFile.get() == 'e')
		{
			Enemies.push_back(std::make_unique<Plane>(wnd.Gfx()));
			Enemies[numEnemies]->setPos((column * 2) - 11, 0, (row * 2) - 6);
			numEnemies++;
			Cubes.push_back(std::make_unique<Cube>(wnd.Gfx()));
			Cubes[numCubes]->setPos((column * 2) - 11, -2, (row * 2) - 6);
			numCubes++;
		}

		column++;
	}
	levelFile.close();
}
