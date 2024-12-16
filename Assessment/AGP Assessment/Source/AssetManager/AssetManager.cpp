#include "AssetManager.h"
#include <WICTextureLoader.h>
ID3D11Device* AssetManager::m_dev = nullptr;
ID3D11DeviceContext* AssetManager::m_devcon = nullptr;

std::unordered_map<const wchar_t*, std::shared_ptr<Material>> AssetManager::m_materials;
std::unordered_map<const wchar_t*, std::shared_ptr<ObjFileModel>> AssetManager::m_models;
std::unordered_map<const wchar_t*, std::shared_ptr<SpriteFont>> AssetManager::m_fonts;


void AssetManager::Initialize(ID3D11Device* dev, ID3D11DeviceContext* devcon)
{
	m_dev = dev;
	m_devcon = devcon;
}

void AssetManager::CleanUp()
{
	//clear all maps 
	GetMaterials().clear();

	if (m_dev)
	{
		delete m_dev;
		m_dev = nullptr;
	}
	if (m_devcon)
	{
		delete m_devcon;
		m_devcon = nullptr;
	}
}

std::shared_ptr<Material> AssetManager::CreateMaterial(const wchar_t* texturePath)
{
	if (IsMaterialLoaded(*texturePath))
	{
		return RetrieveMaterial(*texturePath);
	}
	else
	{
		ID3D11ShaderResourceView* texture = nullptr;
		DirectX::CreateWICTextureFromFile(m_dev, m_devcon, texturePath, nullptr, &texture);
		std::shared_ptr<Material> material = std::make_shared<Material>(m_dev, m_devcon, texture);
		GetMaterials().insert(std::make_pair(texturePath, material));
		return material;
	}

}

std::shared_ptr<ObjFileModel> AssetManager::CreateModel(const wchar_t* modelPath)
{
	if (IsModelLoaded(*modelPath))
	{
		return RetrieveModel(*modelPath);
	}
	else
	{
		std::shared_ptr<ObjFileModel> model = std::make_shared<ObjFileModel>((char*)modelPath, m_dev, m_devcon);
		GetModels().insert(std::make_pair(modelPath, model));
		return model;

	}
	
}

std::shared_ptr<SpriteFont> AssetManager::MakeFont(const wchar_t* fontPath)
{
	return std::shared_ptr<SpriteFont>();
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

bool AssetManager::IsModelLoaded(const wchar_t& modelPath)
{
	for (const auto& model : GetModels())
	{
		if (wcscmp(model.first, &modelPath) == 0)
		{
			return true;
		}
	}
}

bool AssetManager::IsFontLoaded(const wchar_t& fontPath)
{
	for (const auto& font : GetFonts())
	{
		if (wcscmp(font.first, &fontPath) == 0)
		{
			return true;
		}
	}
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

std::shared_ptr<ObjFileModel> AssetManager::RetrieveModel(const wchar_t& modelPath)
{
	auto it = GetModels().find(&modelPath);
	if (it != GetModels().end())
	{
		return it->second;
	}
	return nullptr; 
}

std::shared_ptr<SpriteFont> AssetManager::RetrieveFont(const wchar_t& fontPath)
{
	auto it = GetFonts().find(&fontPath);
	if (it != GetFonts().end())
	{
		return it->second;
	}
}

