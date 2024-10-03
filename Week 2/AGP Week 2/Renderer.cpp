#include "Renderer.h"

HRESULT Renderer::InitRenderer(HWND hWnd, int ScreenHeight, int ScreenWidth)
{
	//Create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd = {};
	//fill the swap chain description struct
	scd.BufferCount = 1;  //one back buffer 
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  //use 32-bit color 
	scd.BufferDesc.Width = ScreenWidth;  //set the back buffer width
	scd.BufferDesc.Height = ScreenHeight;  //set the back buffer height
	scd.BufferDesc.RefreshRate.Numerator = 60;  // 60 FPS
	scd.BufferDesc.RefreshRate.Denominator = 1;  // 60/1 = 60 FPS
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // how the swap chain should be used
	scd.OutputWindow = hWnd;  // the window to be used
	scd.SampleDesc.Count = 1;  // how many samples for AA
	scd.Windowed = TRUE;  // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;  // allow full-screen switching


	HRESULT hr;
	//create a device, device context and swap chain using the information in the scd struct
	hr = D3D11CreateDeviceAndSwapChain(NULL, //use default adapter
		D3D_DRIVER_TYPE_HARDWARE,            //use hardware acceleration, can also use software or WARP renderers
		NULL,                                //for software driver type
		D3D11_CREATE_DEVICE_DEBUG,           // flags can be OR'D together. we are enabling debug for better warnings and errors 
		NULL,                                //direct3d feature levels. NULL will use d3d11.0 or older.
		NULL,                                // size of array passed to above member - NULL since we passed NULL
		D3D11_SDK_VERSION,                   // always set to D3D11_SDK_VERSION 
		&scd,                               //pointer to our swap chain description
		&g_swapChain,                          //pointer to our swap chain COM object
		&g_dev,                              //pointer to our device 
		NULL,                                //Out param - will be set to chosen feature level 
		&g_devcon);                          // pointer to our immediate device context

	if (FAILED(hr))
		return hr;

	//get the address of the back buffer
	ID3D11Texture2D* pBackBufferTexture = nullptr;

	//get the back buffer from the swap chain
	hr = g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBufferTexture);
	if (FAILED(hr))
		return hr;

	hr = g_dev->CreateRenderTargetView(pBackBufferTexture, NULL, &g_backBuffer);
	if (FAILED(hr))
		return hr;

	pBackBufferTexture->Release();

	g_devcon->OMSetRenderTargets(1, &g_backBuffer, NULL);


	//set the viewport
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = ScreenWidth;
	viewport.Height = ScreenHeight;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	g_devcon->RSSetViewports(1, &viewport);

	return S_OK;
}

void Renderer::CleanRenderer()
{
	if (g_backBuffer) g_backBuffer->Release();
	if (g_swapChain) g_swapChain->Release();
	if (g_dev) g_dev->Release();
	if (g_devcon) g_devcon->Release();
}

void Renderer::RenderFrame()
{
	g_devcon->ClearRenderTargetView(g_backBuffer, clearColour);

	//do 3D rendering on the back buffer here
	// 
	//change colour based on asnyc key press
	/*if (GetAsyncKeyState('A') & 0x8000)
	{
		float color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
		g_devcon->ClearRenderTargetView(g_backBuffer, color);
	}
	else if (GetAsyncKeyState('S') & 0x8000)
	{
		float color[4] = { 0.0f, 1.0f, 0.0f, 1.0f };

		g_devcon->ClearRenderTargetView(g_backBuffer, color);
	}
	else if (GetAsyncKeyState('D') & 0x8000)
	{
		float color[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
		g_devcon->ClearRenderTargetView(g_backBuffer, color);
	}
	else
	{
		float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		g_devcon->ClearRenderTargetView(g_backBuffer, color);
	}*/
	//ChooseRandomColour();  //gives me a seizure 

		//flip the back buffer and the front buffer. display on screen
		g_swapChain->Present(0, 0);

}

void Renderer::SetClearColour(float r, float g, float b)
{
	clearColour[0] = r;
	clearColour[1] = g;
	clearColour[2] = b;
	clearColour[3] = 1.0f;
}

void Renderer::ChooseRandomColour()
{
	clearColour[0] = (float)(rand() % 100) / 100;
	clearColour[1] = (float)(rand() % 100) / 100;
	clearColour[2] = (float)(rand() % 100) / 100;
	clearColour[3] = 1.0f;
}
