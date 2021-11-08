#pragma once

#include <cstdint>
#include <DirectXMath.h>

class Camera
{
public:

	Camera() = default;
	Camera(const Camera& camera) = delete;
	Camera& operator=(const Camera& camera) = delete;
	~Camera() = default;

	void UpdateCamera();
	void SetYaw(float new_yaw);
	void SetPitch(float new_pitch);

private:
	
	DirectX::XMVECTOR camTarget;
	DirectX::XMVECTOR camPosition;
	DirectX::XMVECTOR camUp;
	DirectX::XMMATRIX camView;

	DirectX::XMVECTOR defaultForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR defaultRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR camForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR camRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	

	DirectX::XMMATRIX camRotationMatrix;
	DirectX::XMMATRIX groundWorld;
	

	float moveLeftRight = 0.0f;
	float moveForwardBack = 0.0f;

	float camYaw = 0.0f;
	float camPitch = 0.0f;
};