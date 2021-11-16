#include "Collision.h"

bool Collision::checkBoundingBox(DirectX::XMFLOAT3 otherPos, float x_size, float y_size, float z_size)
{
	return (pos.x + width >= otherPos.x - x_size && pos.x - width <= otherPos.x + x_size) &&
		(pos.y + height >= otherPos.y - y_size && pos.y - height <= otherPos.y + y_size) &&
		(pos.z + depth >= otherPos.z - z_size && pos.z - depth <= otherPos.z + z_size);
}
bool Collision::checkBoundingSphere(DirectX::XMFLOAT3 otherPos, float x_size, float y_size, float z_size)
{
	float distX = pos.x - otherPos.x;

	float distY = pos.y - otherPos.y;

	float distance = (distX * distX) + (distY * distY);

	return distance < radius * radius;
}

void Collision::setBoundingBox(DirectX::XMFLOAT3 boxPos)
{
	pos = boxPos;
}

void Collision::setBoundingSphere(DirectX::XMFLOAT3 spherePos, float _radius)
{
	pos = spherePos;
	radius = _radius;
}
