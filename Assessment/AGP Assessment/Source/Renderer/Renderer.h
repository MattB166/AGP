#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "../Constant Buffer/CBuffer.h"
using namespace DirectX;
class Renderer
{

public:
	HRESULT InitRenderer(HWND hwnd, int width, int height);
	
	void CleanRenderer();

	void Clear(); // current scene as parameters? 

	void Present(); 

	ID3D11Device* GetDevice() { return m_device; }

	ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext; }

	ID3D11Buffer* GetCBuffer() { return m_CBuffer; }

private:
	IDXGISwapChain* m_swapChain = nullptr;

	ID3D11Device* m_device = nullptr;

	ID3D11DeviceContext* m_deviceContext = nullptr;

	ID3D11RenderTargetView* m_backBuffer = nullptr;

	D3D11_VIEWPORT m_viewport = {};

	ID3D11InputLayout* m_inputLayout = nullptr;

	ID3D11Buffer* m_CBuffer = nullptr;

	ID3D11DepthStencilState* m_depthStencilState = nullptr;

	ID3D11DepthStencilState* pDepthWriteSolid = nullptr;

	ID3D11DepthStencilView* m_zBuffer = nullptr;

	ID3D11RasterizerState* m_rasterizerState = nullptr;

	XMMATRIX m_worldMatrix; //world matrix
	
	XMMATRIX m_projectionMatrix; //projection matrix. camera manages this and view matrix. 

	XMMATRIX m_viewMatrix; //view matrix


	//current camera 
};

