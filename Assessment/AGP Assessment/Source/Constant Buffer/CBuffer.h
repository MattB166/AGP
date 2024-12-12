#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

	struct CBuffer
	{

		XMMATRIX WVP;
		XMMATRIX WV;
		XMVECTOR ambientLightCol; // 16 bytes
		XMVECTOR directionalLightDir; // 16 bytes
		XMVECTOR directionalLightCol; // 16 bytes
		//PointLight pointLights[MAX_POINT_LIGHTS];


	};
	struct ReflectiveCBuffer
	{

		XMMATRIX WVP;
		XMMATRIX WV;
		XMVECTOR ambientLightCol; // 16 bytes
		XMVECTOR directionalLightDir; // 16 bytes
		XMVECTOR directionalLightCol; // 16 bytes
		//PointLight pointLights[MAX_POINT_LIGHTS];


	};


