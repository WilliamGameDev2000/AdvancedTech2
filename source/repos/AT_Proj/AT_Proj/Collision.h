#pragma once
#include <DirectXCollision.h>

class Collision
{
public:
	bool checkBoundingBox(DirectX::XMFLOAT3 otherPos, float x_size, float y_size, float z_size);
	bool checkBoundingSphere(DirectX::XMFLOAT3 otherPos, float x_size, float y_size, float z_size);
	void setBoundingBox(DirectX::XMFLOAT3 boxPos);
	void setBoundingSphere(DirectX::XMFLOAT3 spherePos, float _radius);
private:
	//general
	DirectX::XMFLOAT3 pos = { 0,0,0 };
	//Box
	float width = 1.1f;
	float height = 1.1f;
	float depth = 1.1f;
	// sphere
	float radius = 0.0f;
};