#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
class Camera
{
public:
	XMMATRIX GetViewMatrix(); // Returns the view matrix
	XMMATRIX GetProjectionMatrix(); // Returns the projection matrix 
	

private:

};

