#include "Camera.h"
#include "../Time/TimeClass.h"
#include <iostream>

Camera::Camera()
{
	//xPos = 0.0f;
	//yPos = 0.0f;
	//zPos = 0.0f;
	//yaw = 0.0f;
	//pitch = 0.0f;
	FOV = 60.0f;
	nearClip = 0.1f;
	farClip = 200.0f;
	SetProjectionMatrix(FOV, 800, 600, nearClip, farClip);

}

void Camera::SetPosition(float x, float y, float z)
{
	xPos = x;
	xPos = y;
	xPos = z;

}

XMMATRIX Camera::GetViewMatrix()
{
	XMVECTOR eyepos = { xPos,yPos,zPos };
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

void Camera::RotateCamera(float pitch, float yaw)
{
	this->pitch += pitch;
	this->yaw += yaw;

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

	XMVECTOR fwd = XMVectorSet(sin(yaw) * sin(pitch), cos(pitch), cos(yaw) * sin(pitch), 0.0f);
	fwd = XMVector3Normalize(fwd);

	XMVECTOR movement = XMVectorScale(fwd, z);

	XMVECTOR Right = XMVectorSet(sin(yaw - XM_PI / 2), 0.0f, cos(yaw - XM_PI / 2), 0.0f);

	Right = XMVector3Normalize(Right);

	movement = XMVectorAdd(movement, XMVectorScale(Right, -x));

	XMVECTOR UpVector = XMVector3Cross(fwd, Right);	
	UpVector = XMVector3Normalize(UpVector);
	movement = XMVectorAdd(movement, XMVectorScale(UpVector, -y));


	xPos += XMVectorGetX(movement); 
	yPos += XMVectorGetY(movement);
	zPos += XMVectorGetZ(movement);

	
	std::cout << "Camera Position: " << xPos << " " << yPos << " " << zPos << std::endl;
}
