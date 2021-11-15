#include "Collision.h"

bool Collision::checkBoundingBox(DirectX::XMFLOAT3 otherPos)
{
	return (pos.x + 1 >= otherPos.x - 1 && pos.x - 1 <= otherPos.x + 1) &&
		(pos.y + 1 >= otherPos.y - 1 && pos.y - 1 <= otherPos.y + 1) &&
		(pos.z + 1 >= otherPos.z - 1 && pos.z - 1 <= otherPos.z + 1);
}

void Collision::setBoundingBox(DirectX::XMFLOAT3 boxPos)
{
	pos = boxPos;
}
