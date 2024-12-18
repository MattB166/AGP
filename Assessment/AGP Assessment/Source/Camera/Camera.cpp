#include "Camera.h"

Camera::Camera()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	yaw = 0.0f;
	//pitch = 0.0f;
	FOV = 60.0f;
	nearClip = 0.1f;
	farClip = 200.0f;
	SetProjectionMatrix(FOV, 800, 600, nearClip, farClip);

}

void Camera::SetPosition(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;

}

XMMATRIX Camera::GetViewMatrix()
{
	XMVECTOR eyepos = { x,y,z };
	XMVECTOR camup = { 0,1,0 };
	XMVECTOR lookat
	{
		sin(yaw) * sin(pitch),
		cos(pitch),
		cos(yaw) * sin(pitch)
	};
	return XMMatrixLookToLH(eyepos, lookat, camup);

}

XMMATRIX Camera::GetProjectionMatrix()
{
	return projection; 
}

void Camera::SetProjectionMatrix(float fov, float screenW, float screenH, float nClip, float fClip)
{
	FOV = fov;
	nearClip = nClip;
	farClip = fClip;
	aspectRatio = screenW / screenH;
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(FOV), aspectRatio, nearClip, farClip);
}

void Camera::YawCamera(float amount)
{
	yaw += amount;
}

void Camera::PitchCamera(float amount)
{
	pitch += amount;

	constexpr float UPPITCH_LIMIT = XMConvertToRadians(179.0f);
	constexpr float LOWPITCH_LIMIT = XMConvertToRadians(1.0f);
	if (pitch < LOWPITCH_LIMIT)
	{
		pitch = LOWPITCH_LIMIT;
	}
	else
		if (pitch > UPPITCH_LIMIT)
		{
			pitch = UPPITCH_LIMIT;
		}
}

void Camera::MoveCamera(float x, float y, float z)
{
	fwd = XMVectorSet(sin(yaw) * sin(pitch), cos(pitch), cos(yaw) * sin(pitch), 0.0f);
	fwd = XMVector3Normalize(fwd);

	XMVECTOR movement = XMVectorScale(fwd, z);

	XMVECTOR newRight = XMVectorSet(sin(yaw - XM_PI / 2), 0.0f, cos(yaw - XM_PI / 2), 0.0f);

	RightVector = XMVector3Normalize(newRight);

	movement = XMVectorAdd(movement, XMVectorScale(RightVector, -x));

	UpVector = XMVector3Cross(fwd, RightVector);	
	UpVector = XMVector3Normalize(UpVector);
	movement = XMVectorAdd(movement, XMVectorScale(UpVector, -y));

	x += XMVectorGetX(movement);
	y += XMVectorGetY(movement);
	z += XMVectorGetZ(movement);


}
