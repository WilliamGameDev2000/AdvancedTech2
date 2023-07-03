#include "Game.h"
#include <memory>
#include <fstream>


Game::Game() : wnd(1024, 720, 0, 0, "AT GAME WINDOW")
{
	last = steady_clock::now();
	LoadLevel("MapFile");

	player = std::make_unique<Player>(wnd.Gfx().cam, wnd.KB, wnd);
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	wnd.Gfx().SetCamera(player->PlayerCam().GetView());
	
	
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
	
	wnd.Gfx().ClearBuffer(0.7f, 0.0f, 0.12f);

	if (wnd.KB.KeyIsPressed(' '))
	{
		if (player->GetAmmoAmount() <= 0)
		{
			player->ReloadGun();
		}
		else
		{
			Bullets.push_back(std::make_unique<Cube>(wnd.Gfx()));
			player->Shoot(Bullets[Bullets.size() - 1]);
		}
	}


	
	for (auto& c : Cubes)
	{
		if (c->isColliding(player->GetPos(), 0.5, 0.5, 0.5))
		{
			if (player->PlayerCam().getForwardBack() != 0 || player->PlayerCam().getLeftRight() != 0)
			{
				player->PlayerCam().Translate(-1 * player->PlayerCam().getLeftRight(), -1 * player->PlayerCam().getForwardBack());
			}
			else
			{
				if (player->GetDir() == Player::direction::forward)
				{
					player->PlayerCam().Translate(0.0f, player->PlayerCam().getForwardBack() - 0.1);
				}
				if (player->GetDir() == Player::direction::back)
				{
					player->PlayerCam().Translate(0.0f, player->PlayerCam().getForwardBack() + 0.1);
				}
				if (player->GetDir() == Player::direction::left)
				{
					player->PlayerCam().Translate(player->PlayerCam().getLeftRight() + 0.1, 0.0f);
				}
				if (player->GetDir() == Player::direction::right)
				{
					player->PlayerCam().Translate(player->PlayerCam().getLeftRight() - 0.1, 0.0f);
				}	
				player->GetDir() == Player::direction::stationary;
			}
			
		}
		c->Draw(wnd.Gfx());
	}
	for (auto& e : Enemies)
	{
		e->Draw(wnd.Gfx());
		e->setRot(wnd.Gfx().cam.getPos());
		e->Update(dt);
	}
	for (auto& B : Bullets)
	{
		//if(shot)
		B->Draw(wnd.Gfx());
		B->Update(dt);
		/*for (auto& c : Cubes)
		{
			if (B->isColliding(c->GetPos(), 0.5, 0.5, 0.5))
			{
				destroy cube from scene but keep in the list?
			}
		}*/
		
	}
	
	player->Move(dt);

	//player->PlayerCam().UpdateCamera();
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
