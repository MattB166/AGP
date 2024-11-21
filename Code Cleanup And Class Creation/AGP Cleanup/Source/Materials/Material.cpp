#include "Material.h"
#include <d3dcompiler.h>
#include <WICTextureLoader.h>

Material::~Material()
{
	if (p_VS) p_VS->Release();
	if (p_PS) p_PS->Release();
	if (p_Layout) p_Layout->Release();
	if (p_CBuffer) p_CBuffer->Release();
	if (p_Texture) p_Texture->Release();
	if (p_Sampler) p_Sampler->Release();
}

HRESULT Material::InitMaterial(ID3D11Device* g_dev, LPCSTR entrypoint,  LPCWSTR vertexshaderfile, LPCWSTR pixelshaderfile, ID3D11InputLayout** il)
{
	HRESULT result;
	ID3DBlob* VS, * pErrorBlob;
	result = D3DCompileFromFile(vertexshaderfile, 0, 0, entrypoint, "vs_4_0", 0, 0, &VS, &pErrorBlob);

	if (FAILED(result))
	{
		OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
		pErrorBlob->Release();
		return result;
	}
	result = g_dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &p_VS);
	if (FAILED(result))
	{
		OutputDebugString(L"Failed to create vertex shader");
		return result;
	}

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	result = g_dev->CreateInputLayout(ied, ARRAYSIZE(ied), VS->GetBufferPointer(), VS->GetBufferSize(), il);
	VS->Release();
	if (FAILED(result))
	{
		OutputDebugString(L"Failed to create input layout");
		return result;
	}


	ID3DBlob* PS;
	result = D3DCompileFromFile(pixelshaderfile, 0, 0, entrypoint, "ps_4_0", 0, 0, &PS, &pErrorBlob);
	if (FAILED(result))
	{
		OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
		pErrorBlob->Release();
		return result;
	}
	result = g_dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &p_PS);
	PS->Release();
	if (FAILED(result))
	{
		OutputDebugString(L"Failed to create pixel shader");
		return result;
	}

	return S_OK;


}

void Material::SetTexture(ID3D11ShaderResourceView* texture, ID3D11Device* dev, ID3D11DeviceContext* devcon, const wchar_t* fileName)
{
	DirectX::CreateWICTextureFromFile(dev, devcon, fileName, NULL, &p_Texture);
}

void Material::SetSampler(ID3D11SamplerState* sampler, ID3D11Device* g_dev)
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; //linear filtering
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; //wrap texture
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; //wrap texture
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; //wrap texture
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX; //max level of detail
	g_dev->CreateSamplerState(&sampDesc, &p_Sampler);
}
