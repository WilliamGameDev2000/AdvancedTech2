#pragma once
#include "Camera.h"
#include "Keyboard.h"
#include "Window.h"

class Player
{
public:
	explicit Player(Camera& cam, Keyboard& keys, Window& wind) : playerCam(cam), input(keys), wnd(wind) {}
	~Player() = default;
	enum class direction { stationary = -1, forward = 0, back = 1, left = 2, right = 3 };

	void Shoot(std::unique_ptr<class Cube> &bullet);
	void ReduceAmmo();
	void ReloadGun();
	void ReduceHealth(float damage);
	void Update(float delta);

	int GetAmmoAmount();
	bool GetCanShoot();

	void SetWindow(class Window window);

	Camera& PlayerCam();
	direction GetDir() { return traveling; }
	XMFLOAT3 GetPos();
	
private:
	direction traveling = direction::stationary;
	void Move();
	void Translate(direction dir);
	Camera& playerCam;
	Keyboard& input;
	Window& wnd;
	XMFLOAT3 playerPos{};
	float health = 100;
	//bullets
	int ammo = 5;
	int clipSize = 5;
	float fire_rate = 1.1f;
	bool can_shoot = true;

	float speed = 2.5f;
	float dt = 0;
};