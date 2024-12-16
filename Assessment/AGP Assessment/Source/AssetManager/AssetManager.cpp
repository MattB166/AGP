#include "AssetManager.h"
#include <WICTextureLoader.h>
ID3D11Device* AssetManager::m_dev = nullptr;
ID3D11DeviceContext* AssetManager::m_devcon = nullptr;

void AssetManager::Initialize(ID3D11Device* dev, ID3D11DeviceContext* devcon)
{
	m_dev = dev;
	m_devcon = devcon;
}

std::shared_ptr<Material> AssetManager::CreateMaterial(ID3D11Device* dev, ID3D11DeviceContext* devcon, const wchar_t* texturePath)
{
	if (IsMaterialLoaded(*texturePath))
	{
		return RetrieveMaterial(*texturePath);
	}
	else
	{
		ID3D11ShaderResourceView* texture = nullptr;
		DirectX::CreateWICTextureFromFile(dev, devcon, texturePath, nullptr, &texture);
		std::shared_ptr<Material> material = std::make_shared<Material>(dev, devcon, texture);
		GetMaterials().insert(std::make_pair(texturePath, material));
		return material;
	}

}

bool AssetManager::IsMaterialLoaded(const wchar_t& texturePath)
{
	for (const auto& material : GetMaterials())
	{
		if (wcscmp(material.first, &texturePath) == 0)
		{
			return true;
		}
	}
	return false;
}

std::shared_ptr<Material> AssetManager::RetrieveMaterial(const wchar_t& texturePath)
{
	auto it = GetMaterials().find(&texturePath);
	if (it != GetMaterials().end())
	{
		return it->second;
	}
	return nullptr; 
}

