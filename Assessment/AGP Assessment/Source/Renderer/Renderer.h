#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "../Constant Buffer/CBuffer.h"
class Renderer
{

public:
	HRESULT InitRenderer(HWND hwnd, int width, int height);
	
	void CleanRenderer();

	void Clear(); 

	void Present(); 

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


	//current camera 
};

