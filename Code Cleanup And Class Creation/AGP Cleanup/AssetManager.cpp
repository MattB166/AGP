#include "AssetManager.h"
#include <WICTextureLoader.h>	

bool AssetManager::IsTextureLoaded(const wchar_t& textureName)
{
	for (const auto& texture : m_textures)
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
	for (const auto& model : m_models)
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
	for (const auto& font : m_fonts)
	{
		if (wcscmp(font.second, &fontName) == 0)
		{
			return true;
		}
	}
	return false;
}

bool AssetManager::IsVertexShaderLoaded(const wchar_t& vertexShaderName)
{
	for (const auto& vertexShader : m_vertexShaders)
	{
		if (wcscmp(vertexShader.second, &vertexShaderName) == 0)
		{
			return true;
		}
	}
	return false;
}

bool AssetManager::IsPixelShaderLoaded(const wchar_t& pixelShaderName)
{
	for (const auto& pixelShader : m_pixelShaders)
	{
		if (wcscmp(pixelShader.second, &pixelShaderName) == 0)
		{
			return true;
		}
	}
}
