#include "Renderer.h"
#include <DirectXColors.h>
#include <iostream>

HRESULT Renderer::InitRenderer(HWND hwnd, int width, int height)
{
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; 

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, NULL, D3D11_SDK_VERSION, &scd, &m_swapChain, &m_device, NULL, &m_deviceContext);

	if (FAILED(hr))
	{
		return hr;
	}

	ID3D11Texture2D* backBuffer = nullptr;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr))
		return hr;

	hr = m_device->CreateRenderTargetView(backBuffer, NULL, &m_backBuffer);
	if (FAILED(hr))
		return hr;

	backBuffer->Release();

	D3D11_TEXTURE2D_DESC zBufferDesc = { 0 };
	zBufferDesc.Width = width;
	zBufferDesc.Height = height;
	zBufferDesc.ArraySize = 1;
	zBufferDesc.MipLevels = 1;
	zBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	zBufferDesc.SampleDesc.Count = scd.SampleDesc.Count;
	zBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	zBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	//create the z-buffer
	ID3D11Texture2D* zBufferTexture = nullptr;
	hr = m_device->CreateTexture2D(&zBufferDesc, NULL, &zBufferTexture);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create z-buffer texture");
		return hr;
	}

	ID3D11Texture2D* pDepthStencil = nullptr;
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = m_device->CreateTexture2D(&texDesc, NULL, &pDepthStencil);

	D3D11_DEPTH_STENCIL_DESC depth = { };
	depth.DepthEnable = TRUE;
	depth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth.DepthFunc = D3D11_COMPARISON_LESS;

	hr = m_device->CreateDepthStencilState(&depth, &m_depthStencilState);

	//create the z-buffer view
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = zBufferDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hr = m_device->CreateDepthStencilView(zBufferTexture, &dsvDesc, &m_zBuffer);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create depth stencil view");
		return hr;
	}




	zBufferTexture->Release();


	m_deviceContext->OMSetRenderTargets(1, &m_backBuffer, m_zBuffer);


	//set the viewport
	m_viewport = {};
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = width;
	m_viewport.Height = height;
	m_viewport.MinDepth = 0;
	m_viewport.MaxDepth = 1;
	m_deviceContext->RSSetViewports(1, &m_viewport);

	//create the constant buffer
	D3D11_BUFFER_DESC cbd = {};
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(CBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	if (FAILED(m_device->CreateBuffer(&cbd, NULL, &m_CBuffer)))
	{
		OutputDebugString(L"Failed to create constant buffer");
		return hr;
	}

	D3D11_RASTERIZER_DESC rsDesc; // creates a rasterizer state for transparency 
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	m_device->CreateRasterizerState(&rsDesc, &m_rasterizerState);
	m_deviceContext->RSSetState(m_rasterizerState);

	//depth stencil states 
	D3D11_DEPTH_STENCIL_DESC dsDesc = { 0 };
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	if (FAILED(m_device->CreateDepthStencilState(&dsDesc, &pDepthWriteSolid)))
	{
		OutputDebugString(L"Failed to create depth stencil state");
		return hr;
	}

	//SKYBOX DEPTH STENCIL STATE WOULD GO HERE BUT PUT IN SKYBOX CLASS INSTEAD 
	 
	return S_OK;
}

void Renderer::CleanRenderer()
{
	if (m_rasterizerState) m_rasterizerState->Release();
	if (pDepthWriteSolid) pDepthWriteSolid->Release();
	if (m_depthStencilState) m_depthStencilState->Release();
	if (m_zBuffer) m_zBuffer->Release();
	if (m_CBuffer) m_CBuffer->Release();
	if (m_inputLayout) m_inputLayout->Release();
	if (m_backBuffer) m_backBuffer->Release();
	if (m_swapChain) m_swapChain->Release();
	if (m_deviceContext) m_deviceContext->Release();
	if (m_device) m_device->Release();
}

void Renderer::Clear()
{
	std::cout << "Clearing" << std::endl;
	m_deviceContext->ClearRenderTargetView(m_backBuffer, DirectX::Colors::DeepSkyBlue);
	m_deviceContext->ClearDepthStencilView(m_zBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	m_deviceContext->OMSetRenderTargets(1, &m_backBuffer, m_zBuffer);
	m_deviceContext->RSSetViewports(1, &m_viewport);
}

void Renderer::Present()
{
	std::cout << "Presenting" << std::endl;
	m_deviceContext->OMSetRenderTargets(1, &m_backBuffer, m_zBuffer);
	m_swapChain->Present(0, 0); 
}
