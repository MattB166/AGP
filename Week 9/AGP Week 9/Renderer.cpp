#include "Renderer.h"
#include "ReadData.h"
#include <d3dcompiler.h>
#include <WICTextureLoader.h>	
#include "TimeClass.h"
#include <DirectXColors.h>



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

	//z-buffer description
	D3D11_TEXTURE2D_DESC zBufferDesc = { 0 };
	zBufferDesc.Width = ScreenWidth;
	zBufferDesc.Height = ScreenHeight;
	zBufferDesc.ArraySize = 1;
	zBufferDesc.MipLevels = 1;
	zBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	zBufferDesc.SampleDesc.Count = scd.SampleDesc.Count;
	zBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	zBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	//create the z-buffer
	ID3D11Texture2D* zBufferTexture = nullptr;
	hr = g_dev->CreateTexture2D(&zBufferDesc, NULL, &zBufferTexture);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create z-buffer texture");
		return hr;
	}

	//create depth stencil dimensions
	ID3D11Texture2D* pDepthStencil = nullptr;
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = ScreenWidth;
	texDesc.Height = ScreenHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = g_dev->CreateTexture2D(&texDesc, NULL, &pDepthStencil);

	D3D11_DEPTH_STENCIL_DESC depth = { };
	depth.DepthEnable = TRUE;
	depth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth.DepthFunc = D3D11_COMPARISON_LESS;

	hr = g_dev->CreateDepthStencilState(&depth, &g_DSS);

	//create the z-buffer view
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = zBufferDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hr = g_dev->CreateDepthStencilView(zBufferTexture, &dsvDesc, &g_ZBuffer);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create depth stencil view");
		return hr;
	}

	


	zBufferTexture->Release();


	g_devcon->OMSetRenderTargets(1, &g_backBuffer, g_ZBuffer);


	//set the viewport
	 viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = ScreenWidth;
	viewport.Height = ScreenHeight;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	g_devcon->RSSetViewports(1, &viewport);

	InitPipeline();
	InitGraphics();
	InitScene();

	

	return S_OK;
}

void Renderer::CleanRenderer()
{
	delete model;
	if (pRasterState) pRasterState->Release();
	if (pAlphaBlendStateDisable) pAlphaBlendStateDisable->Release();
	if (pAlphaBlendStateEnable) pAlphaBlendStateEnable->Release();
	//if (pText) delete pText;
	if (pTexture) pTexture->Release();
	if (pSampler) pSampler->Release();
	if (g_DSS) g_DSS->Release();
	if (g_ZBuffer) g_ZBuffer->Release();
	if (pIndexBuffer) pIndexBuffer->Release();
	if (pCBuffer) pCBuffer->Release();
	if (pVBuffer) pVBuffer->Release();
	if (pVS) pVS->Release();
	if (pPS) pPS->Release();
	if (pLayout) pLayout->Release();

	if (g_backBuffer) g_backBuffer->Release();
	if (g_swapChain) g_swapChain->Release();
	if (g_dev) g_dev->Release();
	if (g_devcon) g_devcon->Release();
}

void Renderer::RenderFrame()
{
	g_devcon->ClearRenderTargetView(g_backBuffer,Colors::DarkSlateGray);
	g_devcon->ClearDepthStencilView(g_ZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	///these 3 here are what allow us to restore the depth buffer after drawing text. 
	g_devcon->OMSetDepthStencilState(g_DSS, 1);
	g_devcon->OMSetRenderTargets(1, &g_backBuffer, g_ZBuffer);
	g_devcon->RSSetViewports(1, &viewport);

	g_devcon->VSSetShader(pVS, 0, 0); //shaders encapsulated in shader objects and set to the device context
	g_devcon->PSSetShader(pPS, 0, 0);
	
	//set the vertex buffer
	UINT stride = sizeof(Vertex); //size of a single vertex
	UINT offset = 0;
	g_devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);  //set the vertex buffer
	g_devcon->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	g_devcon->IASetInputLayout(pLayout); //input layout is set to the device context, so GPU knows how to read the vertex buffer

	//set primitive topology
	g_devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //triangle list is the primitive topology

	//g_devcon->RSSetState(pRasterState);

	g_devcon->PSSetSamplers(0, 1, &pSampler);
	g_devcon->PSSetShaderResources(0, 1, &pTexture);



	XMMATRIX world, view, projection;
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(60),SCREEN_WIDTH / (float)SCREEN_HEIGHT,0.1f, 100);
	view = cam.GetViewMatrix();

	world = cube1.GetWorldMatrix();
	CBUFFER0 cBuffer;
	cBuffer.WVP = world * view * projection;

	///lighting 
	//ambient lighting
	cBuffer.ambientLightCol = ambientLightColour;
	//directional lighting
	cBuffer.directionalLightCol = directionalLightColour;

	XMMATRIX transpose = XMMatrixTranspose(world); //transpose rotations
	cBuffer.directionalLightDir = XMVector3Transform(directionalLightShinesFrom, transpose); //transform the light direction by the world matrix

	//point lighting
	/*cBuffer.pointLightCol = pointLightColour;
	cBuffer.pointLightPos = pointLightPosition;
	cBuffer.pointLightStrength = pointLightStrength;*/

	for (size_t i = 0; i < MAX_POINT_LIGHTS;i++)
	{
		if (!pointLights[i].enabled)
			continue;

		XMMATRIX inverse = XMMatrixInverse(nullptr, world);
		cBuffer.pointLights[i].position = XMVector3Transform(pointLights[i].position, inverse);
		cBuffer.pointLights[i].colour = pointLights[i].colour;
		cBuffer.pointLights[i].strength = pointLights[i].strength;
		cBuffer.pointLights[i].enabled = pointLights[i].enabled;
	}

	
	g_devcon->UpdateSubresource(pCBuffer, 0, 0, &cBuffer, 0, 0);
	g_devcon->VSSetConstantBuffers(0, 1, &pCBuffer);


	g_devcon->PSSetShaderResources(0, 1, &pTexture);
	g_devcon->PSSetSamplers(0, 1, &pSampler);
	
	//g_devcon->Draw(3, 0); //draw the vertex buffer to the back buffer
	//g_devcon->DrawIndexed(36, 0, 0); 
	model->Draw();

	world = cube2.GetWorldMatrix();
	cBuffer.WVP = world * view * projection;
	g_devcon->UpdateSubresource(pCBuffer, 0, 0, &cBuffer, 0, 0);   //////SECOND CUBE RENDERING 
	g_devcon->VSSetConstantBuffers(0, 1, &pCBuffer);
	//g_devcon->DrawIndexed(36, 0, 0);
	model->Draw();   ////loads in the model.obj file as the model to be drawn for the second cube 

	//pText->AddText("Hello World", -1, +1, 0.075f); //adds text and sets the position of the text 
	//g_devcon->OMSetBlendState(pAlphaBlendStateEnable, 0, 0xffffffff);
	//pText->RenderText();
	//g_devcon->OMSetBlendState(pAlphaBlendStateDisable, 0, 0xffffffff);    ///old text way 


	//draw text  in a better way. use this for UI later on down the line 
	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), L"Hello, World!", DirectX::XMFLOAT2(100, 50), DirectX::Colors::ForestGreen);
	spriteFont2->DrawString(spriteBatch.get(), L"Hello, World!", DirectX::XMFLOAT2(10, 10), DirectX::Colors::OrangeRed);
	spriteBatch->End();
	g_devcon->OMSetRenderTargets(1, &g_backBuffer, g_ZBuffer); ///messes up depth buffer if not set back to the back buffer 
	
		//flip the back buffer and the front buffer. display on screen
	g_swapChain->Present(0, 0);

}

void Renderer::InitScene()
{
	cube2.pos = { 3.0f,0.0f,3.0f };
	//cube2.rot = { XMConvertToRadians(30),XMConvertToRadians(45),0.0f};
	/*cube1.pos = { 0,0,1 };
	cube1.scl = { 1,1,1 };
	cube1.rot = { 0.0f,0.0f,XMConvertToRadians(90)};*/

	pointLights[0] = { XMVECTOR{1.5f,0,-1},{0.9f,0,0.85f,1},100,true };
	pointLights[1] = { XMVECTOR{-1.5f,0,-1},{0,0.9f,0.85f,1},200,true };
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

HRESULT Renderer::InitPipeline()
{


	auto vertexShaderByteCode = DX::ReadData(L"Compiled Shaders/VertexShader.cso"); //read the compiled vertex shader code
	auto pixelShaderByteCode = DX::ReadData(L"Compiled Shaders/PixelShader.cso"); //read the compiled pixel shader code
	
	
	//encapsulate both shaders into shader objects 
	result = g_dev->CreateVertexShader(vertexShaderByteCode.data(), vertexShaderByteCode.size(), NULL, &pVS); //create the vertex shader from bytecode
   if (FAILED(result))
   {
	   OutputDebugString(L"Failed to create vertex shader");
	   return result;
   }
   result = g_dev->CreatePixelShader(pixelShaderByteCode.data(), pixelShaderByteCode.size(), NULL, &pPS);
   if (FAILED(result))
   {
	   OutputDebugString(L"Failed to create pixel shader");
	   return result;
   }

   D3DReflect(vertexShaderByteCode.data(), vertexShaderByteCode.size(), IID_ID3D11ShaderReflection, (void**)&vShaderReflection); //shader reflection is used to retrieve info
   //about the shader, such as input and output parameters, resources, constants etc.
   vShaderReflection->GetDesc(&desc);

   D3D11_SIGNATURE_PARAMETER_DESC* signatureParamDescriptions = new D3D11_SIGNATURE_PARAMETER_DESC[desc.InputParameters]{ 0 };
   //input paramters of the vertex shader are described in the signature. includes semantic name, semantic index, register, system value type, component type, mask and read write mask
   for (UINT i = 0; i < desc.InputParameters; i++)
   {
	   vShaderReflection->GetInputParameterDesc(i, &signatureParamDescriptions[i]);
   }
	//set the shader objects 
   //g_devcon->VSSetShader(pVS, 0, 0); //shaders encapsulated in shader objects and set to the device context
	//g_devcon->PSSetShader(pPS, 0, 0);

	//create input layout description
	D3D11_INPUT_ELEMENT_DESC* ied = new D3D11_INPUT_ELEMENT_DESC[desc.InputParameters]{ 0 }; //input layout is created
	//based on the input parameters of the vertex shader. layout describes how the vertex buffer is to be read
	for (size_t i = 0; i < desc.InputParameters; i++)
	{
		ied[i].SemanticName = signatureParamDescriptions[i].SemanticName;
		ied[i].SemanticIndex = signatureParamDescriptions[i].SemanticIndex;
		if (signatureParamDescriptions[i].ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
		{
			switch (signatureParamDescriptions[i].Mask)
			{
			case 1:
				ied[i].Format = DXGI_FORMAT_R32_FLOAT;
				break;
			case 3:
				ied[i].Format = DXGI_FORMAT_R32G32_FLOAT;
				break;
			case 7:
				ied[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
				break;
			case 15:
				ied[i].Format = DXGI_FORMAT_R32G32B32A32_UINT;
				break;
			default:
				break;
			}
		}
		ied[i].InputSlot = 0;
		ied[i].AlignedByteOffset = D3D10_APPEND_ALIGNED_ELEMENT;
		ied[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ied[i].InstanceDataStepRate = 0;
	}
	/*{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},

	};*/

	result = g_dev->CreateInputLayout(ied, desc.InputParameters, vertexShaderByteCode.data(), vertexShaderByteCode.size(), &pLayout);

	if (FAILED(result))
	{
		OutputDebugString(L"Failed to create input layout");
		return result;
	}

	

	delete[] signatureParamDescriptions;
	delete[] ied;
	return S_OK;
}

void Renderer::InitGraphics()
{
	//create a triangle using the VERTEX struct
	Vertex v[] =
	{
		{XMFLOAT3{-0.5f,-0.5f,-0.5f},XMFLOAT4{DirectX::Colors::White},XMFLOAT2{0.0f,1.0f},XMFLOAT3{-0.5773f,-0.5773f,-0.5773f}}, //FRONT BOTTOM LEFT
		{XMFLOAT3{-0.5f,0.5f,-0.5f},XMFLOAT4{DirectX::Colors::White},XMFLOAT2{0.0f,0.0f},XMFLOAT3{-0.5773f,0.5773f,-0.5773f}}, //FRONT TOP LEFT
		{XMFLOAT3{0.5f,0.5f,-0.5f},XMFLOAT4{DirectX::Colors::White},XMFLOAT2{1.0f,0.0f},XMFLOAT3{0.5773f,0.5773f,-0.5773f}}, //FRONT TOP RIGHT
		{XMFLOAT3{0.5f,-0.5f,-0.5f},XMFLOAT4{DirectX::Colors::White},XMFLOAT2{1.0f,1.0f},XMFLOAT3{0.5773f,-0.5773f,-0.5773f}}, //FRONT BOTTOM RIGHT

		{XMFLOAT3{-0.5f,-0.5f,0.5f},XMFLOAT4{DirectX::Colors::White},XMFLOAT2{0.0f,1.0f},XMFLOAT3{-0.5773f,-0.5773f,0.5773f}}, //BACK BOTTOM LEFT
		{XMFLOAT3{-0.5f,0.5f,0.5f},XMFLOAT4{DirectX::Colors::White},XMFLOAT2{0.0f,0.0f},XMFLOAT3{-0.5773f,0.5773f,0.5773f}}, //BACK TOP LEFT
		{XMFLOAT3{0.5f,0.5f,0.5f},XMFLOAT4{DirectX::Colors::White},XMFLOAT2{1.0f,0.0f},XMFLOAT3{0.5773f,0.5773f,0.5773f}}, //BACK TOP RIGHT
		{XMFLOAT3{0.5f,-0.5f,0.5f},XMFLOAT4{DirectX::Colors::White},XMFLOAT2{1.0f,1.0f},XMFLOAT3{0.5773f,-0.5773f,0.5773f}}, //BACK BOTTOM RIGHT 


	};

	unsigned int indices[] = {/*front*/ 0,1,2,2,3,0, /*back*/ 7,6,5,5,4,7,/*left*/ 4,5,1,1,0,4,/*right*/
							  3,2,6,6,7,3,/*top*/ 1,5,6,6,2,1, /*bottom*/ 4,0,3,3,7,4 };

	//create the vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.Usage = D3D11_USAGE_DYNAMIC; // dynamic allows cpu write access and gpu read access
	bd.ByteWidth = sizeof(v); //size of the buffer in bytes - size of the vertex struct * 3 vertices
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // allow cpu to write in buffer

	g_dev->CreateBuffer(&bd, NULL, &pVBuffer); //create the buffer

	if (pVBuffer == 0)
	{
		return;
	}

	D3D11_BUFFER_DESC bufferDesc = {0};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(indices);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = indices;
	
	if (FAILED(g_dev->CreateBuffer(&bufferDesc, &initData, &pIndexBuffer)))
	{
		OutputDebugString(L"Failed to create index buffer");
		return;
	}


	D3D11_BUFFER_DESC cbd = { };
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(CBUFFER0);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	if (FAILED(g_dev->CreateBuffer(&cbd, NULL, &pCBuffer)))
	{
		OutputDebugString(L"Failed to create constant buffer");
		return;
	}

	//copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	g_devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); // map the buffer
	memcpy(ms.pData, v, sizeof(v)); //copy the data
	g_devcon->Unmap(pVBuffer, NULL); // unmap the buffer

	CreateWICTextureFromFile(g_dev, g_devcon, L"Box.bmp", NULL, &pTexture); /// load texture from file 

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; //linear filtering
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; //wrap texture
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; //wrap texture
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; //wrap texture
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX; //max level of detail
	g_dev->CreateSamplerState(&sampDesc, &pSampler); //create the sampler state
	//creates a linear sampler state. 


	//pText = new Text2D("font1.png", g_dev, g_devcon);
	D3D11_BLEND_DESC bd1 = { 0 };
	bd1.RenderTarget[0].BlendEnable = TRUE;
	bd1.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd1.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd1.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd1.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd1.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd1.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd1.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	bd1.IndependentBlendEnable = FALSE;
	bd1.AlphaToCoverageEnable = FALSE;
	g_dev->CreateBlendState(&bd1, &pAlphaBlendStateEnable);

	D3D11_BLEND_DESC bd2 = { 0 };
	bd2.RenderTarget[0].BlendEnable = FALSE;
	bd2.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd2.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd2.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd2.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd2.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd2.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd2.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	bd2.IndependentBlendEnable = FALSE;
	bd2.AlphaToCoverageEnable = FALSE;
	g_dev->CreateBlendState(&bd2, &pAlphaBlendStateDisable);

	D3D11_RASTERIZER_DESC rsDesc; // creates a rasterizer state for transparency 
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	g_dev->CreateRasterizerState(&rsDesc, &pRasterState);
	g_devcon->RSSetState(pRasterState);

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(g_devcon);
	spriteFont = std::make_unique<DirectX::SpriteFont>(g_dev, L"Fonts/matura_mt_script.spritefont");
	spriteFont2 = std::make_unique<DirectX::SpriteFont>(g_dev, L"Fonts/comic_sans_ms_16.spritefont");

	model = new ObjFileModel{ (char*)"cube.obj",g_dev,g_devcon };


}
void Renderer::MoveCamera(float x,float y,float z)
{
	XMVECTOR fwd = XMVectorSet(sin(cam.yaw) * sin(cam.pitch), cos(cam.pitch), cos(cam.yaw) * sin(cam.pitch), 0.0f);
	//move into lookto struct inside camera class when come to make it 
	//i.e. struct lookto{x,y,z};
	// { static_cast<float> sin(cam.yaw) * sin(cam.pitch);
	//   static_cast<float> cos(cam.pitch);
	//   static_cast<float> cos(cam.yaw) * sin(cam.pitch); 
	// }
	

	fwd = XMVector3Normalize(fwd);
	XMVECTOR movement = XMVectorScale(fwd, z);
	
	XMVECTOR right = XMVectorSet(sin(cam.yaw - XM_PI/2), 0.0f, cos(cam.yaw - XM_PI/2), 0.0f);
	
	right = XMVector3Normalize(right);

	movement = XMVectorAdd(movement, XMVectorScale(right, -x));
	
	cam.x += XMVectorGetX(movement);
	cam.y += XMVectorGetY(movement);
	cam.z += XMVectorGetZ(movement);


	//need to clamp pitch to prevent camera flipping. might be able to put this somewhere else inside cam class for cleanliness 

	
}
void Renderer::RotateCamera(float pitch, float yaw)
{
	cam.pitch += pitch;
	cam.yaw += yaw;

	///need to clamp camera to prevent flipping 
	 //clamp Pitch
	constexpr float PITCH_LIMIT = XMConvertToRadians(179.0f);
	if (cam.pitch < -PITCH_LIMIT)
	{
		cam.pitch = -PITCH_LIMIT;
	}
	else
		if (cam.pitch > PITCH_LIMIT)
		{
			cam.pitch = PITCH_LIMIT;
		}


}
void Renderer::RotateCube(float x, float y, float z)
{
	cube1.rot.x += x;
	cube1.rot.y += y;
	cube1.rot.z += z;

}
	/*Order of Operations
		1.	InitPipeline:
	•	Sets up shaders.
		•	Creates and sets the input layout.
		2.	InitGraphics :
		•	Creates the vertex buffer and fills it with data.
		3.	RenderFrame :
		•	Binds the vertex buffer.
		•	Sets the primitive topology.
		•	Issues the draw call.
}*/
