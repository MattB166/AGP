#include "AssetManager.h"
#include <d3dcompiler.h>
#include <WICTextureLoader.h>	

std::shared_ptr<Material> AssetManager::CreateTexture(const wchar_t* textureName, ID3D11Device* dev, ID3D11VertexShader* VS, ID3D11PixelShader* PS)
{
	if (IsTextureLoaded(*textureName))
	{
		return RetrieveTexture(*textureName);
	}
	auto material = std::make_shared<Material>(textureName, dev, VS, PS);

	GetTextures().insert(std::make_pair(textureName, material));

	return material;  ////should prevent dangling pointer by doing it this way? and can only remove the pointer from the map when the material is deleted by its last reference.

}

ObjFileModel* AssetManager::CreateModel(const wchar_t* modelName, ID3D11Device* g_dev, ID3D11DeviceContext* g_devcon)
{
	if (IsModelLoaded(*modelName))
	{
		return RetrieveModel(*modelName);
	}
	ObjFileModel* model; 
	model = new ObjFileModel{ (char*)modelName, g_dev, g_devcon };
	GetModels().insert(std::make_pair(modelName, model));
	return model;
}

SpriteFont* AssetManager::MakeFont(ID3D11Device* g_dev, const wchar_t* fontName)
{
	if (IsFontLoaded(*fontName))
	{
		return RetrieveFont(*fontName);
	}
	std::unique_ptr<DirectX::SpriteFont> font = std::make_unique<DirectX::SpriteFont>(g_dev, fontName); //make sure to release this when done
	SpriteFont* fontPtr = font.get();
	GetFonts().insert(std::make_pair(fontName, font.release()));
	return fontPtr; 
}

ID3D11VertexShader* AssetManager::CreateVertexShader(ID3D11Device* g_dev, const wchar_t* vertexShaderName, LPCSTR entrypoint, ID3D11InputLayout** il)
{
	if (IsVertexShaderLoaded(*vertexShaderName))
	{
		return RetrieveVertexShader(*vertexShaderName);
	}

	HRESULT hr;
	ID3DBlob* VS, * pErrorBlob;
	ID3D11VertexShader* vertexShader = nullptr;
	hr = D3DCompileFromFile(vertexShaderName, 0, 0, entrypoint, "vs_4_0", 0, 0, &VS, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
			return nullptr;
		}
	}
		hr = g_dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, &vertexShader);
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
		
		hr = g_dev->CreateInputLayout(ied, ARRAYSIZE(ied), VS->GetBufferPointer(), VS->GetBufferSize(), il);
		VS->Release();
		if (FAILED(hr))
		{
			OutputDebugString(L"Failed to create input layout");
			return nullptr;
		}
		GetVertexShaders().insert(std::make_pair(vertexShaderName, vertexShader));
		return vertexShader;

}

ID3D11PixelShader* AssetManager::CreatePixelShader(ID3D11Device* g_dev, const wchar_t* pixelShaderName, LPCSTR entrypoint)
{
	if (IsPixelShaderLoaded(*pixelShaderName))
	{
		return RetrievePixelShader(*pixelShaderName);
	}

	HRESULT hr;
	ID3DBlob* PS, * pErrorBlob;
	ID3D11PixelShader* pixelShader = nullptr;
	hr = D3DCompileFromFile(pixelShaderName, 0, 0, entrypoint, "ps_4_0", 0, 0, &PS, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
			return nullptr;
		}
	}
	hr = g_dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, &pixelShader);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create pixel shader");
		PS->Release();
		return nullptr;
	}
	PS->Release();
	GetPixelShaders().insert(std::make_pair(pixelShaderName, pixelShader));
	return pixelShader;

}

bool AssetManager::IsTextureLoaded(const wchar_t& textureName)
{
	for (const auto& texture : GetTextures())
	{
		if (wcscmp(texture.first, &textureName) == 0)
		{
			return true;
		}
	}
	return false;
}

bool AssetManager::IsModelLoaded(const wchar_t& modelName)
{
	for (const auto& model : GetModels())
	{
		if (wcscmp(model.first, &modelName) == 0)
		{
			return true;
		}
	}
	return false;
}

bool AssetManager::IsFontLoaded(const wchar_t& fontName)
{
	for (const auto& font : GetFonts())
	{
		if (wcscmp(font.first, &fontName) == 0)
		{
			return true;
		}
	}
	return false;
}

bool AssetManager::IsVertexShaderLoaded(const wchar_t& vertexShaderName)
{
	for (const auto& vertexShader : GetVertexShaders())
	{
		if (wcscmp(vertexShader.first, &vertexShaderName) == 0)
		{
			return true;
		}
	}
	return false;
}

bool AssetManager::IsPixelShaderLoaded(const wchar_t& pixelShaderName)
{
	for (const auto& pixelShader : GetPixelShaders())
	{
		if (wcscmp(pixelShader.first, &pixelShaderName) == 0)
		{
			return true;
		}
	}
	return false;
}

ID3D11VertexShader* AssetManager::RetrieveVertexShader(const wchar_t& vertexShaderName)
{
	for (const auto& vertexShader : GetVertexShaders())
	{
		if (wcscmp(vertexShader.first, &vertexShaderName) == 0)
		{
			return vertexShader.second;
		}
	}
	return nullptr;
}

ID3D11PixelShader* AssetManager::RetrievePixelShader(const wchar_t& pixelShaderName)
{
	for (const auto& pixelShader : GetPixelShaders())
	{
		if (wcscmp(pixelShader.first, &pixelShaderName) == 0)
		{
			return pixelShader.second;
		}
	}
	return nullptr;
}

std::shared_ptr<Material> AssetManager::RetrieveTexture(const wchar_t& textureName)
{
	auto it = GetTextures().find(&textureName);
	if (it != GetTextures().end())
	{
		return it->second; 
	}
	return nullptr;
}

ObjFileModel* AssetManager::RetrieveModel(const wchar_t& modelName)
{
	for (const auto& model : GetModels())
	{
		if (wcscmp(model.first, &modelName) == 0)
		{
			return model.second;
		}
	}
	return nullptr;
}

SpriteFont* AssetManager::RetrieveFont(const wchar_t& fontName)
{
	for (const auto& font : GetFonts())
	{
		if (wcscmp(font.first, &fontName) == 0)
		{
			return font.second;
		}
	}
	return nullptr;
}
