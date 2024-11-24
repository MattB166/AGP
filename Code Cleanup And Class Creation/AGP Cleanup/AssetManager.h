#pragma once
#include "../AGP Cleanup/Source/Materials/Material.h"
#include <vector>
#include "../AGP Cleanup/Source/ObjectModel/objfilemodel.h"
#include "SpriteFont.h"
#include <cwchar>
#include <unordered_map>
using namespace DirectX;
class AssetManager //handles storing and loading of assets (textures, models, sounds etc). 
{
public:
	static Material* LoadTexture(const wchar_t& textureName); 
	static ObjFileModel* LoadModel(const wchar_t& modelName); 
	static SpriteFont* LoadFont(const wchar_t& fontName);


private:
	AssetManager() = delete; 
	~AssetManager() = delete;
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;

	static bool IsTextureLoaded(const wchar_t& textureName);
	static bool IsModelLoaded(const wchar_t& modelName);
	static bool IsFontLoaded(const wchar_t& fontName);
	static bool IsVertexShaderLoaded(const wchar_t& vertexShaderName);
	static bool IsPixelShaderLoaded(const wchar_t& pixelShaderName);
	
	static std::unordered_map<Material*, const wchar_t*> m_textures; //map of textures and their names
	static std::unordered_map<ObjFileModel*, const wchar_t*> m_models; //map of models and their names 
	static std::unordered_map<SpriteFont*, const wchar_t*> m_fonts; //map of fonts and their names 
	static std::unordered_map<ID3D11VertexShader*, const wchar_t*> m_vertexShaders; //map of vertex shaders and their names
	static std::unordered_map<ID3D11PixelShader*, const wchar_t*> m_pixelShaders; //map of pixel shaders and their names

	 
};

