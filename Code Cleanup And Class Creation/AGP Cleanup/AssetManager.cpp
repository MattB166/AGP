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
	return nullptr;
}

bool AssetManager::IsTextureLoaded(const wchar_t& textureName)
{
	for (const auto& texture : GetTextures())
	{
		if (wcscmp(texture.second, &textureName) == 0)
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
		if (wcscmp(model.second, &modelName) == 0)
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
		if (wcscmp(font.second, &fontName) == 0)
		{
			return true;
		}
	}
}

bool AssetManager::IsVertexShaderLoaded(const wchar_t& vertexShaderName)
{
	for (const auto& vertexShader : GetVertexShaders())
	{
		if (wcscmp(vertexShader.second, &vertexShaderName) == 0)
		{
			return true;
		}
	}
}

bool AssetManager::IsPixelShaderLoaded(const wchar_t& pixelShaderName)
{
	for (const auto& pixelShader : GetPixelShaders())
	{
		if (wcscmp(pixelShader.second, &pixelShaderName) == 0)
		{
			return true;
		}
	}
}

ID3D11VertexShader* AssetManager::RetrieveVertexShader(const wchar_t& vertexShaderName)
{
	for (const auto& vertexShader : GetVertexShaders())
	{
		if (wcscmp(vertexShader.second, &vertexShaderName) == 0)
		{
			return vertexShader.first;
		}
	}
}

ID3D11PixelShader* AssetManager::RetrievePixelShader(const wchar_t& pixelShaderName)
{
	for (const auto& pixelShader : GetPixelShaders())
	{
		if (wcscmp(pixelShader.second, &pixelShaderName) == 0)
		{
			return pixelShader.first;
		}
	}
}

Material* AssetManager::RetrieveTexture(const wchar_t& textureName)
{
	for (const auto& texture : GetTextures())
	{
		if (wcscmp(texture.second, &textureName) == 0)
		{
			return texture.first;
		}
	}
}

ObjFileModel* AssetManager::RetrieveModel(const wchar_t& modelName)
{
	for (const auto& model : GetModels())
	{
		if (wcscmp(model.second, &modelName) == 0)
		{
			return model.first;
		}
	}
}

SpriteFont* AssetManager::RetrieveFont(const wchar_t& fontName)
{
	for (const auto& font : GetFonts())
	{
		if (wcscmp(font.second, &fontName) == 0)
		{
			return font.first;
		}
	}
}
