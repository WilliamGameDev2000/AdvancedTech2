#include "Camera.h"

void Camera::UpdateCamera()
{
	camRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget = DirectX::XMVector3TransformCoord(defaultForward, camRotationMatrix);
	camTarget = DirectX::XMVector3Normalize(camTarget);

	DirectX::XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = DirectX::XMMatrixRotationY(camYaw);

	camRight = DirectX::XMVector3TransformCoord(defaultRight, RotateYTempMatrix);
	camUp = DirectX::XMVector3TransformCoord(camUp, RotateYTempMatrix);
	camForward = DirectX::XMVector3TransformCoord(defaultForward, RotateYTempMatrix);

	camPosition = DirectX::XMVectorAdd(camPosition, DirectX::XMVectorScale(camRight, moveLeftRight));
	camPosition = DirectX::XMVectorAdd(camPosition, DirectX::XMVectorScale(camForward, moveForwardBack));

	moveLeftRight = 0.0f;
	moveForwardBack = 0.0f;

	camTarget = DirectX::XMVectorAdd(camPosition, camTarget);

	camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, {1,1,1});
}

void Camera::SetYaw(float new_yaw)
{
	camYaw += new_yaw;
}

void Camera::SetPitch(float new_pitch)
{
	camPitch += new_pitch;
}