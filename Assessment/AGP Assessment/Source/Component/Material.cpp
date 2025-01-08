#include "Material.h"
#include <WICTextureLoader.h>
std::vector<std::string> Material::m_AvailableTextureNames = {};
std::unordered_map<std::string, std::string> Material::m_textureNameToPath = {};

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
	//m_AvailableTextureNames.clear();
	//m_textureNameToPath.clear();

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
	ImGui::Text("Current Material: %s", std::string(m_name.begin(), m_name.end()).c_str());
}

void Material::LoadAllTextureNames(const std::string& path)
{
	namespace fs = std::experimental::filesystem;
	for (const auto& entry : fs::directory_iterator(path))
	{
		if (fs::is_regular_file(entry) && entry.path().extension() == ".bmp")
		{
			std::string filename = entry.path().filename().string();
			std::string textureName = filename.substr(0, filename.find_last_of('.'));
			m_AvailableTextureNames.push_back(textureName);
			m_textureNameToPath.insert(std::make_pair(textureName, entry.path().string()));
			std::cout << "Loaded Texture Name : " << textureName << "\n";
		}
	}
}

std::string Material::GetComponentFilePath(const std::string& name) const
{
	auto it = m_textureNameToPath.find(name);
	if (it != m_textureNameToPath.end())
	{
		return it->second;
	}
	return "";
}
