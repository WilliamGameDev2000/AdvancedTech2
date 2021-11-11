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

	moveLeftRight = 0.0f;
	moveForwardBack = 0.0f;

	camTarget = camPosition + camTarget;

	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
}

void Camera::SetYaw(float new_yaw)
{
	camYaw += new_yaw * 0.01f;
}

void Camera::SetPitch(float new_pitch)
{
	camPitch += new_pitch * 0.01f;
	if (camPitch >= 1)
	{
		camPitch = 1;
	}
	else if (camPitch <= -1)
	{
		camPitch = -1;
	}
}

void Camera::Translate(float x, float y, float z)
{
	moveLeftRight += x;
	moveForwardBack += z;
	/*moveLeftRight += y;*/
}

void Camera::Rotate(float x, float y, float z)
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
}
