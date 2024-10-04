#include <d3d11.h>
#include <DirectXMath.h>
#include <d3d11shader.h>
using namespace DirectX;
#pragma once
struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 colour;
};
class Renderer
{

public:

	HRESULT InitRenderer(HWND hWnd, int ScreenHeight, int ScreenWidth);
	void CleanRenderer();
	void RenderFrame();
	void SetClearColour(float r, float g, float b);
	void ChooseRandomColour();
	HRESULT InitPipeline();
	void InitGraphics();

private:
	IDXGISwapChain* g_swapChain = NULL;             // the pointer to the swap chain interface - this is a chain of screen buffers that we draw to and "flip" to display on the monitor. 
	ID3D11Device* g_dev = NULL;                     // the pointer to our Direct3D device interface - the virtual display adapter aka graphics card driver. this object is used to create and allocate resources on the adapter. 
	ID3D11DeviceContext* g_devcon = NULL;           // the pointer to our Direct3D device context - used to control the settings and states of the device, and issue rendering commands to the device.
	ID3D11RenderTargetView* g_backBuffer = NULL;    // the pointer to our back buffer - the final render target that we draw to. this is a 2D texture that is drawn to by the rasterizer stage of the pipeline.
	float clearColour[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	ID3D11VertexShader* pVS = NULL; // the vertex shader
	ID3D11PixelShader* pPS = NULL; // the pixel shader
	ID3D11InputLayout* pLayout = NULL; //pointer to input layout 
	HRESULT result;
	ID3D11Buffer* pVBuffer = NULL; // the vertex buffer
	ID3D11ShaderReflection* vShaderReflection = NULL;
	D3D11_SHADER_DESC desc;




};

