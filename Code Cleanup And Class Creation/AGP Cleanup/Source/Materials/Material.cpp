#include "Material.h"
#include <d3dcompiler.h>
#include <WICTextureLoader.h>

Material::Material(const wchar_t* filename, ID3D11Device* g_dev, ID3D11VertexShader* VS, ID3D11PixelShader* PS)
{
	
}

Material::~Material()
{
	//if (p_Layout) p_Layout->Release();
	//if (p_VertexShader) p_VertexShader->Release();
	//if (p_PixelShader) p_PixelShader->Release();
	if (p_Texture) p_Texture->Release();
	if (p_Sampler) p_Sampler->Release();
}

void Material::SetTexture(ID3D11Device* dev, ID3D11DeviceContext* devcon, const wchar_t* fileName)
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

void Material::Apply(ID3D11DeviceContext* devcon)
{
}
