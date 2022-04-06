#include "Player.h"



void Player::ReduceAmmo()
{
	ammo--;
}

void Player::ReduceHealth(float damage)
{
	health -= damage;
}