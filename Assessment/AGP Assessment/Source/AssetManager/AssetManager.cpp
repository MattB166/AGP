#include "AssetManager.h"
#include <WICTextureLoader.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <iostream>
ID3D11Device* AssetManager::m_dev = nullptr;
ID3D11DeviceContext* AssetManager::m_devcon = nullptr;

std::unordered_map<const wchar_t*, std::shared_ptr<Material>> AssetManager::m_materials;
std::unordered_map<const wchar_t*, std::shared_ptr<Model>> AssetManager::m_models;
std::unordered_map<const wchar_t*, std::shared_ptr<SpriteFont>> AssetManager::m_fonts;
std::unordered_map<std::string, std::shared_ptr<ShaderSet>> AssetManager::m_shaderSets;


void AssetManager::Initialize(ID3D11Device* dev, ID3D11DeviceContext* devcon)
{
	m_dev = dev;
	m_devcon = devcon;

	CreateShaderSet(L"VertexShader.cso", L"PixelShader.cso");
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

std::shared_ptr<Model> AssetManager::CreateModel(const wchar_t* modelPath)
{
	if (IsModelLoaded(*modelPath))
	{
		return RetrieveModel(*modelPath);
	}
	else
	{
		ObjFileModel* Tmpmodel = new ObjFileModel((char*)modelPath, m_dev,m_devcon);
		std::shared_ptr<Model> model = std::make_shared<Model>(m_dev, m_devcon, Tmpmodel);
		GetModels().insert(std::make_pair(modelPath, model));
		return model;

	}
	
}

std::shared_ptr<SpriteFont> AssetManager::MakeFont(const wchar_t* fontPath)
{
	return std::shared_ptr<SpriteFont>();
}

std::shared_ptr<ShaderSet> AssetManager::CreateShaderSet(const wchar_t* vsPath, const wchar_t* psPath)
{
	if (IsShaderSetLoaded(*vsPath, *psPath))
	{
		std::cout << "Shader set already loaded" << std::endl;
		return RetrieveShaderSet(*vsPath, *psPath);
	}
	else
	{
		std::cout << "Creating new shader set" << std::endl;

		auto vsData = DX::ReadData(vsPath);
		auto psData = DX::ReadData(psPath);
		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;

		if (FAILED(m_dev->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader)))
		{
			OutputDebugString(L"Failed to create vertex shader");
		}
		if (FAILED(m_dev->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader)))
		{
			OutputDebugString(L"Failed to create pixel shader");
		}
		///add pixel shader to map instantly. 
		//shader reflection member
		ID3D11ShaderReflection* m_vertexShaderReflection;

		if (FAILED(D3DReflect(vsData.data(), vsData.size(), IID_ID3D11ShaderReflection, (void**)&m_vertexShaderReflection)))
		{
			OutputDebugString(L"Failed to reflect vertex shader");
		}

		D3D11_SHADER_DESC shaderDesc;
		m_vertexShaderReflection->GetDesc(&shaderDesc);

		D3D11_SIGNATURE_PARAMETER_DESC* signatureParamDescriptions = new D3D11_SIGNATURE_PARAMETER_DESC[shaderDesc.InputParameters]{ 0 };
		for (UINT i = 0; i < shaderDesc.InputParameters; i++)
		{
			m_vertexShaderReflection->GetInputParameterDesc(i, &signatureParamDescriptions[i]);
		}
		//maybe set devcon shaders here. try without for now 

		D3D11_INPUT_ELEMENT_DESC* ied = new D3D11_INPUT_ELEMENT_DESC[shaderDesc.InputParameters]{ 0 };
		for (size_t i = 0; i < shaderDesc.InputParameters; i++)
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

		ID3D11InputLayout* inputLayout;
		if (FAILED(m_dev->CreateInputLayout(ied, shaderDesc.InputParameters, vsData.data(), vsData.size(), &inputLayout)))
		{
			OutputDebugString(L"Failed to create input layout");
		}

		//add vertex shader and input layout to map as a pair. 
		auto shaderSet = std::make_shared<ShaderSet>(m_dev, m_devcon, vertexShader, pixelShader, inputLayout);
		std::string key = GenerateKeyForShaderSet(vsPath, psPath);
		GetShaderSets().insert(std::make_pair(key, shaderSet));


		delete[] signatureParamDescriptions;
		delete[] ied;
		delete m_vertexShaderReflection;

		return shaderSet;

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

bool AssetManager::IsModelLoaded(const wchar_t& modelPath)
{
	for (const auto& model : GetModels())
	{
		if (wcscmp(model.first, &modelPath) == 0)
		{
			return true;
		}
	}
	return false;
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
	return false;
}

bool AssetManager::IsShaderSetLoaded(const wchar_t& vsPath, const wchar_t& psPath)
{
	std::string key = GenerateKeyForShaderSet(&vsPath, &psPath);
	return GetShaderSets().begin() != GetShaderSets().end();

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

std::shared_ptr<Model> AssetManager::RetrieveModel(const wchar_t& modelPath)
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

std::shared_ptr<ShaderSet> AssetManager::RetrieveShaderSet(const wchar_t& vsPath, const wchar_t& psPath)
{
	std::string key = GenerateKeyForShaderSet(&vsPath, &psPath);
	auto it = GetShaderSets().find(key);
	return (it != GetShaderSets().end()) ? it->second : nullptr;
}

std::string AssetManager::GenerateKeyForShaderSet(const wchar_t* vsPath, const wchar_t* psPath)
{
	std::wstring key = std::wstring(vsPath) +  L"| " + std::wstring(psPath);
	return std::string(key.begin(), key.end());
}

