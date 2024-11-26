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
	///CALLED ELSEWHERE TO CREATE ASSETS - INTERAL FUNCTIONS MANAGE WHETHER ITS ALREADY BEEN LOADED OR NOT 
	static Material* CreateTexture(const wchar_t& textureName); 
	static ObjFileModel* CreateModel(const wchar_t& modelName); 
	static SpriteFont* MakeFont(const wchar_t& fontName);
	static ID3D11VertexShader* CreateVertexShader(const wchar_t& vertexShaderName, ID3D11VertexShader** vs, ID3D11InputLayout** il);
	static ID3D11PixelShader* CreatePixelShader(const wchar_t& pixelShaderName); 


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



	static ID3D11VertexShader* RetrieveVertexShader(const wchar_t& vertexShaderName);
	static ID3D11PixelShader* RetrievePixelShader(const wchar_t& pixelShaderName);
	static Material* RetrieveTexture(const wchar_t& textureName);
	static ObjFileModel* RetrieveModel(const wchar_t& modelName);
	static SpriteFont* RetrieveFont(const wchar_t& fontName);
	
	//had to do some mad shit to make sure these only get initialised on first use otherwise whole window doesnt even open 
	static std::unordered_map<Material*, const wchar_t*>& GetTextures() { static std::unordered_map<Material*, const wchar_t*> m_textures; return m_textures; }
	static std::unordered_map<ObjFileModel*, const wchar_t*>& GetModels() { static std::unordered_map<ObjFileModel*, const wchar_t*> m_models; return m_models; }
	static std::unordered_map<SpriteFont*, const wchar_t*>& GetFonts() { static std::unordered_map<SpriteFont*, const wchar_t*> m_fonts; return m_fonts; }
	static std::unordered_map<ID3D11VertexShader*, const wchar_t*>& GetVertexShaders() { static std::unordered_map<ID3D11VertexShader*, const wchar_t*> m_vertexShaders; return m_vertexShaders; }
	static std::unordered_map<ID3D11PixelShader*, const wchar_t*>& GetPixelShaders() { static std::unordered_map<ID3D11PixelShader*, const wchar_t*> m_pixelShaders; return m_pixelShaders; }

	 
};

