#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
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
	
	XMMATRIX WVP; // 64 bytes world view projection matrix
	// the 64 comes from each row being 16 bytes 
	// and 4 rows in total. 4*16 = 64 bytes 
	//4,4,4,4
	//4,4,4,4
	//4,4,4,4
	//4,4,4,4
	//xmmatrix is a stricly aligned type for simd hardware 
	//simple instruction multiple data 
	XMVECTOR ambientLightCol; // 16 bytes
	XMVECTOR directionalLightDir; // 16 bytes
	XMVECTOR directionalLightCol; // 16 bytes
	PointLight pointLights[MAX_POINT_LIGHTS];


};