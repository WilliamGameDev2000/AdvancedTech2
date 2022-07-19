#pragma once

#include "Window.h"
#include "Cube.h"
#include "Plane.h"
#include "Player.h"
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
	void LoadLevel(std::string levelFile);
	void LoadWall(std::string levelFile);
	void LoadFloor(std::string levelFile);
	void LoadEnemies(std::string levelFile);

	/*enum direction {stationary = -1, forward = 0, back = 1, left = 2, right = 3};
	direction traveling = stationary;*/

	Window wnd;
	std::chrono::steady_clock::time_point last;
	float rotation;
	int numCubes = 0;
	int numEnemies = 0;
	int numBullets = 0;
	int row = 0;
	int column = 0;
	char wallBlock = '#';
	char floorBlock = '-';

	std::unique_ptr<Player> player;
	float speed = 2.5f;
	std::vector<std::unique_ptr<class Cube>> Cubes;
	std::vector<std::unique_ptr<class Plane>> Enemies;
	std::vector<std::unique_ptr<class Cube>> Bullets;
};
