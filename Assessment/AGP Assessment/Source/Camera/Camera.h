#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
class Camera //this is the scene camera. what we use when editing/viewing the scene. 
{
public:
	Camera(); 

	void SetPosition(float xVal, float yVal, float zVal); // Sets the position of the camera 

	XMMATRIX GetViewMatrix(); // Returns the view matrix

	XMMATRIX GetProjectionMatrix(); // Returns the projection matrix 

	void SetProjectionMatrix(float fov, float screenW, float screenH, float nClip, float fClip); // Sets the projection matrix

	void RotateCamera(float yaw, float pitch); // Rotates the camera around the x axis

	void MoveCamera(float x, float y, float z); // Moves the camera


	///XMVECTOR GetForwardVector() { return fwd; }

	//XMVECTOR GetRightVector() { return RightVector; }

	//XMVECTOR GetUpVector() { return UpVector; } 

	float GetX() { return xPos; }	

	float GetY() { return yPos; }

	float GetZ() { return zPos; }





private:
	float xPos = 0, yPos = 0 , zPos = 0; // The position of the camera

	float pitch = XM_PIDIV2; // The pitch of the camera

	float yaw = 0; // The yaw of the camera

	float FOV; // The field of view of the camera

	float nearClip;

	float farClip;

	float aspectRatio;

	float sensitivity = XM_2PI * 0.045f;

	float speed = 50.0f;

	//XMVECTOR fwd;

	//XMVECTOR RightVector;

	//XMVECTOR UpVector; 

	XMMATRIX projection; // The projection matrix of the camera
};

