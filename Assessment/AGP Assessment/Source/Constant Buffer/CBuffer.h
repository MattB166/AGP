#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#define MAX_POINT_LIGHTS 8
struct PointLight
{
	XMVECTOR position = { 0,0,0,1 };
	XMVECTOR colour = { 1,1,1,1 };
	float strength = 10;
	bool enabled = false;
	float padding[2];
};
	struct CBuffer
	{

		XMMATRIX WVP;
		XMMATRIX WV;
		XMVECTOR ambientLightCol; // 16 bytes
		XMVECTOR directionalLightDir; // 16 bytes
		XMVECTOR directionalLightCol; // 16 bytes
		PointLight pointLights[MAX_POINT_LIGHTS];


	};
	struct ReflectiveCBuffer
	{

		XMMATRIX WVP;
		XMMATRIX WV;
		XMVECTOR ambientLightCol; // 16 bytes
		XMVECTOR directionalLightDir; // 16 bytes
		XMVECTOR directionalLightCol; // 16 bytes
		PointLight pointLights[MAX_POINT_LIGHTS];


	};


