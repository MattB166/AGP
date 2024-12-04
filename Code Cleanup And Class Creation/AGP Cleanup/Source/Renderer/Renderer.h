#include <d3d11.h>
#include <DirectXMath.h>
#include "../Text/Text2D.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "../ObjectModel/ObjFileModel.h"
#include <d3d11shader.h>
#include "../GameObject/GameObject.h"
#include "../CBuffers/CBuffer.h"
#include <DDSTextureLoader.h>
using namespace DirectX;
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_POINT_LIGHTS 8
#pragma once
//struct PointLight
//{
//	XMVECTOR position = { 0,0,0,1 };
//	XMVECTOR colour = { 1,1,1,1 };
//	float strength = 10;
//	bool enabled = false;
//	float padding[2];
//};
struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 colour;
	XMFLOAT2 UV; 
	XMFLOAT3 normal;
};
//struct CBUFFER0
//{
//	XMMATRIX WVP; // 64 bytes world view projection matrix
//	              // the 64 comes from each row being 16 bytes 
//				  // and 4 rows in total. 4*16 = 64 bytes 
//	              //4,4,4,4
//				  //4,4,4,4
//				  //4,4,4,4
//				  //4,4,4,4
//	              //xmmatrix is a stricly aligned type for simd hardware 
//				  //simple instruction multiple data 
//	XMVECTOR ambientLightCol; // 16 bytes
//	XMVECTOR directionalLightDir; // 16 bytes
//	XMVECTOR directionalLightCol; // 16 bytes
//	PointLight pointLights[MAX_POINT_LIGHTS];
//
//	
//};
struct CBufferSkyBox
{
	XMMATRIX wvp;
};
struct Camera
{
	float x = 0, y = 0, z = 0;
	float pitch = XM_PIDIV2, yaw = 0;

	XMMATRIX GetViewMatrix()
	{
		XMVECTOR eyepos = { x,y,z };
		XMVECTOR camup = { 0,1,0 };
		XMVECTOR lookat
		{
			sin(yaw) * sin(pitch),
			cos(pitch),
			cos(yaw) * sin(pitch)
		};
		return XMMatrixLookToLH(eyepos, lookat, camup);
	}
};

//struct Transform // gone into gameobject.h 
//{
//	XMFLOAT3 pos{ 0,0,2 };
//	XMFLOAT3 rot{ 0,0,0 };
//	XMFLOAT3 scl{ 1,1,1 };
//
//	XMMATRIX GetWorldMatrix()
//	{
//		XMMATRIX translation = XMMatrixTranslation(pos.x, pos.y, pos.z);
//		XMMATRIX rotationX = XMMatrixRotationX(rot.x);
//		XMMATRIX rotationY = XMMatrixRotationY(rot.y);
//		XMMATRIX rotationZ = XMMatrixRotationZ(rot.z);
//		XMMATRIX scale = XMMatrixScaling(scl.x, scl.y, scl.z);
//		XMMATRIX world = scale * rotationX * rotationY * rotationZ * translation;
//		return world;
//	}
//};
class Renderer
{

public:

	HRESULT InitRenderer(HWND hWnd, int ScreenHeight, int ScreenWidth);
	void CleanRenderer();
	void RenderFrame();
	void InitScene();
	/*void SetClearColour(float r, float g, float b);
	void ChooseRandomColour();*/
	HRESULT InitPipeline();
	void InitGraphics();
	void MoveCamera(float x, float y, float z); //will be part of game class. move current cam 
	void RotateCamera(float pitch, float yaw); // ditto 
	void RotateCube(float x, float y, float z);
	Camera cam;
	HRESULT LoadVertexShader(LPCWSTR filename, LPCSTR entrypoint, ID3D11VertexShader** vs, ID3D11InputLayout** il);
	HRESULT LoadPixelShader(LPCWSTR filename, LPCSTR entrypoint, ID3D11PixelShader** ps);
	void DrawSkyBox(); 

private:
	IDXGISwapChain* g_swapChain = NULL;             // the pointer to the swap chain interface - this is a chain of screen buffers that we draw to and "flip" to display on the monitor. 
	ID3D11Device* g_dev = NULL;                     // the pointer to our Direct3D device interface - the virtual display adapter aka graphics card driver. this object is used to create and allocate resources on the adapter. 
	ID3D11DeviceContext* g_devcon = NULL;           // the pointer to our Direct3D device context - used to control the settings and states of the device, and issue rendering commands to the device.
	ID3D11RenderTargetView* g_backBuffer = NULL;    // the pointer to our back buffer - the final render target that we draw to. this is a 2D texture that is drawn to by the rasterizer stage of the pipeline.
	float clearColour[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	D3D11_VIEWPORT viewport = {};
	//ID3D11VertexShader* pVS = NULL; // the vertex shader MATERIAL 
	//ID3D11PixelShader* pPS = NULL; // the pixel shader MATERIAL 
	ID3D11InputLayout* pLayout = NULL; //pointer to input layout MATERIAL 
	//HRESULT result; // MATERIAL??
	ID3D11Buffer* pVBuffer = NULL; // the vertex buffer //MATERIAL 
	ID3D11Buffer* pCBuffer = NULL; // the constant buffer //MATERIAL 
	ID3D11Buffer* pIndexBuffer = NULL; // the index buffer	//OBJ DEALS WITH THIS 
	ID3D11DepthStencilState* g_DSS = NULL; // the pointer to our depth stencil state
	ID3D11DepthStencilView* g_ZBuffer = NULL; // the pointer to our depth buffer
	ID3D11ShaderReflection* vShaderReflection = NULL; //MATERIAL 
	D3D11_SHADER_DESC desc;
	//CBuffer cBuffer; // the constant buffer //MATERIAL 
	
	//XMMATRIX projection; //projection matrix 
	

	GameObject* obj1;
	GameObject* obj2;
	


	ObjFileModel* model;
	ObjFileModel* modelSkyBox;

	//ID3D11ShaderResourceView* pTexture = NULL; // the pointer to the texture  //MATERIAL 
	//ID3D11SamplerState* pSampler = NULL; // the pointer to the sampler state  //MATERIAL 


	//Text2D* pText; // the pointer to the text object
	ID3D11BlendState* pAlphaBlendStateEnable = NULL; // the pointer to the alpha blend state. needs to move to a font class 
	ID3D11BlendState* pAlphaBlendStateDisable = NULL; // the pointer to the alpha blend state

	ID3D11RasterizerState* pRasterState = NULL; // the pointer to the rasterizer state 

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;

	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	std::unique_ptr<DirectX::SpriteFont> spriteFont2;


	XMVECTOR ambientLightColour = { 0.1f,0.1f,0.1f,0.01f }; // 16 bytes
	XMVECTOR directionalLightShinesFrom = { 0.2788f,0.7063f, 0.6506f }; // 16 bytes
	XMVECTOR directionalLightColour = { 0.96f,0.8f,0.75f,1.0f };  // 16 bytes
	

	//FOLLOWING ARE ALL PART OF THE SKYBOX MATERIAL CLASS 

	ID3D11RasterizerState* p_RasterSolid = NULL;
	ID3D11RasterizerState* p_RasterSkyBox = NULL;
	ID3D11DepthStencilState* pDepthWriteSolid = NULL;
	ID3D11DepthStencilState* pDepthWriteSkyBox = NULL;
	ID3D11Buffer* pSkyBoxCBuffer = NULL;
	ID3D11ShaderResourceView* pSkyBoxTexture = NULL;
	ID3D11VertexShader* pVSSkyBox = NULL;
	ID3D11PixelShader* pPSSkyBox = NULL;
	ID3D11InputLayout* pLayoutSkyBox = NULL;

	//////MOST OF THIS STUFF WILL NOT BE STAYING HERE 
};
