#include "Camera.h"


XMMATRIX Camera::GetView() const noexcept
{
	return camView;
}

void Camera::SetView(XMMATRIX view)
{
	camView = view;
}

void Camera::UpdateCamera()
{
	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget = XMVector3TransformCoord(defaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);

	camRight = XMVector3TransformCoord(defaultRight, RotateYTempMatrix);
	camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(defaultForward, RotateYTempMatrix);

	camPosition += moveLeftRight * camRight;
	camPosition += moveForwardBack * camForward;
	XMStoreFloat3(&floatPos, camPosition);

	moveLeftRight = 0.0f;
	moveForwardBack = 0.0f;

	camTarget = camPosition + camTarget;

	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
}


void Camera::Translate(float x, float z)
{
	moveLeftRight += x;
	moveForwardBack += z;
}

float Camera::getForwardBack()
{
	return moveForwardBack;
}

float Camera::getLeftRight()
{
	return moveLeftRight;
}

XMFLOAT3 Camera::getPos()
{
	return floatPos;
}

void Camera::Rotate(float x, float y)
{
	camPitch += y;
	camYaw += x;
	if (camPitch >= 1)
	{
		camPitch = 1;
	}
	else if (camPitch <= -1)
	{
		camPitch = -1;
	}

	float max = 3.130872f * 2;
	float min = -3.130872f * 2;
	if (camYaw >= max)
	{
		camYaw = 0;
	}
	else if (camYaw <= min)
	{
		camYaw = 0;
	}
}

float Camera::getYaw()
{
	return camYaw;
}
