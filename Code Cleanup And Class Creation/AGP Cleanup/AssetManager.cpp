#include "AssetManager.h"
#include <WICTextureLoader.h>	

Material* AssetManager::CreateTexture(const wchar_t& textureName)
{
	return nullptr;
}

ObjFileModel* AssetManager::CreateModel(const wchar_t& modelName)
{
	return nullptr;
}

SpriteFont* AssetManager::MakeFont(const wchar_t& fontName)
{
	return nullptr;
}

ID3D11VertexShader* AssetManager::CreateVertexShader(const wchar_t& vertexShaderName, ID3D11VertexShader** vs, ID3D11InputLayout** il)
{
	HRESULT hr;
	ID3DBlob* pVS, * pErrorBlob;


}

ID3D11PixelShader* AssetManager::CreatePixelShader(const wchar_t& pixelShaderName, ID3D11PixelShader** ps)
{
	return nullptr;
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
}

Material* AssetManager::RetrieveTexture(const wchar_t& textureName)
{
	for (const auto& texture : GetTextures())
	{
		if (wcscmp(texture.first, &textureName) == 0)
		{
			return texture.second;
		}
	}
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
}
