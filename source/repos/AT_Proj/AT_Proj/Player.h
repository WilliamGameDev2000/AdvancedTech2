#pragma once
#include "Camera.h"
#include "Keyboard.h"

class Player
{
public:
	explicit Player(Camera& cam, Keyboard& keys) : playerCam(cam), input(keys) {}
	~Player() = default;
	enum class direction { stationary = -1, forward = 0, back = 1, left = 2, right = 3 };

	void Shoot();
	void ReduceAmmo();
	void ReduceHealth(float damage);
	void Move(float delta);
	Camera& PlayerCam();
	direction GetDir() { return traveling; }
	XMFLOAT3 GetPos();
	
private:
	direction traveling = direction::stationary;
	void Translate(direction dir);
	Camera& playerCam;
	Keyboard& input;
	XMFLOAT3 playerPos{};
	float health = 100;
	int ammo = 25;
	//int clipSize = 5;
	float speed = 2.5f;
	float dt = 0;
};