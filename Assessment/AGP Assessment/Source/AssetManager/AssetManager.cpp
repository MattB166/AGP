#include "AssetManager.h"
#include <WICTextureLoader.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <DDSTextureLoader.h>
#include <iostream>
ID3D11Device* AssetManager::m_dev = nullptr;
ID3D11DeviceContext* AssetManager::m_devcon = nullptr;

std::unordered_map<const wchar_t*, std::shared_ptr<Material>> AssetManager::m_materials;
std::unordered_map<const wchar_t*, std::shared_ptr<Model>> AssetManager::m_models;
std::unordered_map<const wchar_t*, std::shared_ptr<SpriteFont>> AssetManager::m_fonts;
std::unordered_map<std::string, std::shared_ptr<ShaderSet>> AssetManager::m_shaderSets;
std::unordered_map<const wchar_t*, std::shared_ptr<ID3D11ShaderResourceView>> AssetManager::m_ddstextures;
std::unordered_map<const wchar_t*, std::shared_ptr<ID3D11VertexShader>> AssetManager::m_vertexShaders;
std::unordered_map<const wchar_t*, std::shared_ptr<ID3D11PixelShader>> AssetManager::m_pixelShaders;
std::unordered_map<const char*, ObjFileModel*> AssetManager::m_objFileModels;
//std::unordered_map<std::string, std::shared_ptr<SkyBox>> AssetManager::m_skyBoxes;



void AssetManager::Initialize(ID3D11Device* dev, ID3D11DeviceContext* devcon)
{
	m_dev = dev;
	m_devcon = devcon;
	CreateShaderSet(L"CompiledShaders/VertexShader.cso", L"CompiledShaders/PixelShader.cso");
	CreateShaderSet(L"CompiledShaders/SkyBoxVShader.cso", L"CompiledShaders/SkyBoxPShader.cso");
	CreateModel(L"Source/SavedModels/cube.obj"); 
	CreateModel(L"Source/SavedModels/Sphere.obj"); 
}

void AssetManager::CleanUp()
{
	//clear all maps 
	GetMaterials().clear();
	GetModels().clear();
	GetFonts().clear();
	GetShaderSets().clear();
	GetDDSTextures().clear();
	GetVertexShaders().clear();

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
		//std::cout << "Model created" << std::endl;
		//std::cout << "Model size: " << GetModels().size() << std::endl;
		return model;

	}
	
}

ObjFileModel* AssetManager::CreateObjFileModel(const char* modelPath)
{
	if (IsObjFileModelLoaded(*modelPath))
	{
		return RetrieveObjFileModel(*modelPath);
	}
	else
	{
		
		//convert wchar_t to char* for the model path.
		ObjFileModel* model;
		model = new ObjFileModel{ (char*)modelPath, m_dev, m_devcon };

		GetObjFileModels().insert(std::make_pair(modelPath, model));
		return model;
	}
}

std::shared_ptr<SpriteFont> AssetManager::MakeFont(const wchar_t* fontPath)
{
	return std::shared_ptr<SpriteFont>();
}

std::shared_ptr<ShaderSet> AssetManager::CreateShaderSet(const wchar_t* vsPath, const wchar_t* psPath)
{
	//print out the path of the shaders.
	//std::wcout << "Vertex shader path: " << vsPath << std::endl;
	//std::wcout << "Pixel shader path: " << psPath << std::endl;
	if (IsShaderSetLoaded(*vsPath, *psPath))
	{
		//std::cout << "Shader set already loaded" << std::endl;
		return RetrieveShaderSet(*vsPath, *psPath);
	}
	else
	{
		//std::cout << "Creating new shader set" << std::endl;

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
					ied[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
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
		std::cout << "Shader set created" << std::endl;
		std::cout << "Shader set size: " << GetShaderSets().size() << std::endl;

		delete[] signatureParamDescriptions;
		delete[] ied;
		if (m_vertexShaderReflection)
			m_vertexShaderReflection->Release();

		return shaderSet;

	}

	
}

std::shared_ptr<ID3D11VertexShader> AssetManager::CreateVertexShader(const wchar_t* vsPath, LPCSTR entrypoint)
{
	if (IsVertexShaderLoaded(*vsPath))
	{
		return RetrieveVertexShader(*vsPath);
	}
	HRESULT hr;
	ID3DBlob* VS, * pErrorBlob;
	ID3D11VertexShader* vertexShader = nullptr;
	hr = D3DCompileFromFile(vsPath, 0, 0, entrypoint, "vs_4_0", 0, 0, &VS, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
			return nullptr;
		}
	}
	hr = m_dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &vertexShader);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create vertex shader");
		VS->Release();
		return nullptr;
	}

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	ID3D11InputLayout* il;
	hr = m_dev->CreateInputLayout(ied, ARRAYSIZE(ied), VS->GetBufferPointer(), VS->GetBufferSize(), &il);
	VS->Release();
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create input layout");
		return nullptr;
	}
	auto vShader = std::shared_ptr<ID3D11VertexShader>(vertexShader);
	GetVertexShaders().insert(std::make_pair(vsPath, vShader));
	return vShader; 

}

std::shared_ptr<ID3D11PixelShader> AssetManager::CreatePixelShader(const wchar_t* psPath, LPCSTR entrypoint)
{
	if (IsPixelShaderLoaded(*psPath))
	{
		return RetrievePixelShader(*psPath);
	}

	HRESULT hr;
	ID3DBlob* PS, * pErrorBlob;
	ID3D11PixelShader* pixelShader = nullptr;
	hr = D3DCompileFromFile(psPath, 0, 0, entrypoint, "ps_4_0", 0, 0, &PS, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
			return nullptr;
		}
	}
	hr = m_dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &pixelShader);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create pixel shader");
		PS->Release();
		return nullptr;
	}
	auto pShader = std::shared_ptr<ID3D11PixelShader>(pixelShader);
	GetPixelShaders().insert(std::make_pair(psPath, pShader));
	return pShader;

}

std::shared_ptr<SkyBox> AssetManager::CreateSkyBox(const wchar_t* texturePath, const char* modelPath, const wchar_t* vsPath, const wchar_t* psPath)
{
	
	bool isObjLoaded = IsObjFileModelLoaded(*modelPath);
	bool isDDSTextureLoaded = IsDDSTextureLoaded(*texturePath);
	bool isShaderSetLoaded = IsShaderSetLoaded(*vsPath, *psPath); 

	ObjFileModel* model;
	if (isObjLoaded)
	{
		model = RetrieveObjFileModel(*modelPath);
		//std::cout << "Model retrieved as already loaded" << std::endl;
	}
	else
	{
		model = CreateObjFileModel(modelPath);
	}
	std::shared_ptr<ID3D11ShaderResourceView> texture;
	if (isDDSTextureLoaded)
	{
		texture = RetrieveDDSTexture(*texturePath);
		//std::cout << "Texture retrieved as already loaded" << std::endl;
	}
	else
	{
		texture = CreateDDSTexture(texturePath);
	}
	std::shared_ptr<ShaderSet> shaderSet;
	if (isShaderSetLoaded)
	{
		shaderSet = RetrieveShaderSet(*vsPath, *psPath);
		//std::cout << "Shader set retrieved as already loaded" << std::endl;
	}
	else
	{
		shaderSet = CreateShaderSet(vsPath, psPath);
	}

	auto skyBox = std::make_shared<SkyBox>(m_dev, m_devcon, model, shaderSet->GetVertexShader(), shaderSet->GetPixelShader(), shaderSet->GetInputLayout(), texture.get(),texturePath);
	std::cout << "Skybox created" << std::endl;
	return skyBox;

}

std::shared_ptr<ID3D11ShaderResourceView> AssetManager::CreateDDSTexture(const wchar_t* texturePath)
{
	assert(texturePath != nullptr && "Texture path is null");
	if (IsDDSTextureLoaded(*texturePath))
	{
		return RetrieveDDSTexture(*texturePath);
	}
	else
	{
		ID3D11ShaderResourceView* texture = nullptr;
		HRESULT hr = DirectX::CreateDDSTextureFromFile(m_dev,m_devcon, texturePath, nullptr, &texture);
		if (FAILED(hr))
		{
			OutputDebugString(L"Failed to create DDS texture");
		}
		auto sharedTexture = std::shared_ptr<ID3D11ShaderResourceView>(texture, [](ID3D11ShaderResourceView* ptr) { if (ptr)ptr->Release(); });

		GetDDSTextures().insert(std::make_pair(texturePath, sharedTexture));
		return sharedTexture;
	}
}

const wchar_t* AssetManager::GetTexturePath(const std::shared_ptr<SkyBox>& skyBox)
{
	const wchar_t* texturePath = skyBox->GetTexturePath();

	auto it = GetDDSTextures().find(texturePath);
	if (it != GetDDSTextures().end())
	{
		return it->first;
	}
	return nullptr;
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
	return GetShaderSets().find(key) != GetShaderSets().end();

}

bool AssetManager::IsDDSTextureLoaded(const wchar_t& texturePath)
{
	for (const auto& texture : GetDDSTextures())
	{
		if (wcscmp(texture.first, &texturePath) == 0)
		{
			return true;
		}
	}
}

bool AssetManager::IsVertexShaderLoaded(const wchar_t& vsPath)
{
	for (const auto& vertexShader : GetVertexShaders())
	{
		if (wcscmp(vertexShader.first, &vsPath) == 0)
		{
			return true;
		}
	}
	return false;
}

bool AssetManager::IsPixelShaderLoaded(const wchar_t& psPath)
{
	for (const auto& pixelShader : GetPixelShaders())
	{
		if (wcscmp(pixelShader.first, &psPath) == 0)
		{
			return true;
		}
	}
}

bool AssetManager::IsObjFileModelLoaded(const char& modelPath)
{
	for (const auto& model : GetObjFileModels())
	{
		if (strcmp(model.first, &modelPath) == 0)
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
	return nullptr;
}

std::shared_ptr<ShaderSet> AssetManager::RetrieveShaderSet(const wchar_t& vsPath, const wchar_t& psPath)
{
	std::string key = GenerateKeyForShaderSet(&vsPath, &psPath);
	auto it = GetShaderSets().find(key);
	return (it != GetShaderSets().end()) ? it->second : nullptr;
}

std::shared_ptr<ID3D11ShaderResourceView> AssetManager::RetrieveDDSTexture(const wchar_t& texturePath)
{
	auto it = GetDDSTextures().find(&texturePath);
	if (it != GetDDSTextures().end())
	{
		return it->second;
	}
}

std::shared_ptr<ID3D11VertexShader> AssetManager::RetrieveVertexShader(const wchar_t& vsPath)
{
	auto it = GetVertexShaders().find(&vsPath);
	if (it != GetVertexShaders().end())
	{
		return it->second;
	}
}

std::shared_ptr<ID3D11PixelShader> AssetManager::RetrievePixelShader(const wchar_t& psPath)
{
	auto it = GetPixelShaders().find(&psPath);
	if (it != GetPixelShaders().end())
	{
		return it->second;
	}
}

ObjFileModel* AssetManager::RetrieveObjFileModel(const char& modelPath)
{
	auto it = GetObjFileModels().find(&modelPath);
	if (it != GetObjFileModels().end())
	{
		return it->second;
	}
}

std::string AssetManager::GenerateKeyForShaderSet(const wchar_t* vsPath, const wchar_t* psPath)
{
	std::wstring key = std::wstring(vsPath) +  L"| " + std::wstring(psPath);
	return std::string(key.begin(), key.end());
}
