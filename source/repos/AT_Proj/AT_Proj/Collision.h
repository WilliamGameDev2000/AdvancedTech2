#pragma once
#include <DirectXCollision.h>

class Collision
{
public:
	bool checkBoundingBox(DirectX::XMFLOAT3 otherPos);
	void setBoundingBox(DirectX::XMFLOAT3 boxPos);
private:
	DirectX::XMFLOAT3 pos = { 0,0,0 };
};