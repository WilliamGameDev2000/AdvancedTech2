#pragma once


class Player
{
public:
	Player() = default;
	~Player() = default;

	void ReduceAmmo();
	void ReduceHealth(float damage);

private:
	float health;
	int ammo;
};