#include "Material.h"
#include <WICTextureLoader.h>
std::vector<std::string> Material::m_AvailableTextureNames;

Material::Material(ID3D11Device* dev, ID3D11DeviceContext* devcon, ID3D11ShaderResourceView* texture, const char* name) : Component(dev, devcon,name,ComponentType::Texture), m_texture(texture)
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	m_dev->CreateSamplerState(&samplerDesc, &m_sampler);
}

Material::Material(ID3D11Device* dev, ID3D11DeviceContext* devcon) : Component(dev, devcon, "Material", ComponentType::Texture)
{
}

Material::~Material()
{
	if (m_texture)
	{
		m_texture->Release();
	}
	if (m_sampler)
	{
		m_sampler->Release();
	}
}

void Material::Apply()
{
	//std::cout << "Applying Material" << std::endl;
	m_devcon->PSSetShaderResources(0, 1, &m_texture);
	m_devcon->PSSetSamplers(0, 1, &m_sampler);
	
}

void Material::ShowDebugWindow()
{
	//std::cout << "Showing Material Debug Window" << std::endl;
	//ImGui::Text("Material");
	ImGui::Text("Material : %s", m_name);
}
