#include "Material.h"
#include <d3dcompiler.h>
#include <WICTextureLoader.h>

Material::Material(const wchar_t* filename, ID3D11Device* g_dev, ID3D11DeviceContext* g_devcon, ID3D11VertexShader* VS, ID3D11PixelShader* PS, ID3D11ShaderResourceView* SRV, ID3D11InputLayout* il)
{
	//SetTexture(g_dev, nullptr, filename); need to add device context to this function 
	p_Texture = SRV;
	ApplyShaders(VS, PS, il);
	SetSampler(g_dev);
	SetTexture(g_dev, g_devcon, filename);
	 //need to add sampler to this function
}

Material::~Material()
{
	//if (p_Layout) p_Layout->Release();
	//if (p_VertexShader) p_VertexShader->Release();
	//if (p_PixelShader) p_PixelShader->Release();
	if (p_Texture) p_Texture->Release();
	if (p_Sampler) p_Sampler->Release();
	if (p_VertexShader) p_VertexShader->Release();
	if (p_PixelShader) p_PixelShader->Release();
}

void Material::SetTexture(ID3D11Device* dev, ID3D11DeviceContext* devcon, const wchar_t* fileName)
{
	DirectX::CreateWICTextureFromFile(dev, devcon, fileName, NULL, &p_Texture); //MAYBE THIS NEEDS TO BE DONE INSIDE ASSET MANAGER TOO? to prevent multiple textures being loaded. 
}

void Material::SetSampler(ID3D11Device* g_dev)
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

void Material::ApplyShaders(ID3D11VertexShader* VS, ID3D11PixelShader* PS, ID3D11InputLayout* il)
{
	m_inputLayout = il;
	p_VertexShader = VS;
	p_PixelShader = PS;
}

void Material::Apply(ID3D11DeviceContext* devcon)
{
	devcon->IASetInputLayout(m_inputLayout);
	devcon->VSSetShader(p_VertexShader, 0, 0);
	devcon->PSSetShader(p_PixelShader, 0, 0);

	devcon->PSSetSamplers(0, 1, &p_Sampler);
	devcon->PSSetShaderResources(0, 1, &p_Texture);
}
