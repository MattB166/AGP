#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
class Camera //this is the scene camera. what we use when editing/viewing the scene. 
{
public:
	Camera(); 

	void SetPosition(float x, float y, float z); // Sets the position of the camera 

	XMMATRIX GetViewMatrix(); // Returns the view matrix

	XMMATRIX GetProjectionMatrix(); // Returns the projection matrix 

	void SetProjectionMatrix(float fov, float screenW, float screenH, float nClip, float fClip); // Sets the projection matrix

	void YawCamera(float amount); // Rotates the camera around the y axis

	void PitchCamera(float amount); // Rotates the camera around the x axis

	void MoveCamera(float x, float y, float z); // Moves the camera


	XMVECTOR GetForwardVector() { return fwd; }

	XMVECTOR GetRightVector() { return RightVector; }

	XMVECTOR GetUpVector() { return UpVector; } 

	float GetX() { return x; }	

	float GetY() { return y; }

	float GetZ() { return z; }





private:
	float x, y, z; // The position of the camera

	float pitch = XM_PIDIV2; // The pitch of the camera

	float yaw = 0; // The yaw of the camera

	float FOV; // The field of view of the camera

	float nearClip;

	float farClip;

	float aspectRatio;

	XMVECTOR fwd;

	XMVECTOR RightVector;

	XMVECTOR UpVector; 

	XMMATRIX projection; // The projection matrix of the camera
};

