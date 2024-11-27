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
	static std::shared_ptr<Material> CreateTexture(const wchar_t* textureName, ID3D11Device* dev, const wchar_t* VS, const wchar_t* PS); 
	static ObjFileModel* CreateModel(const wchar_t* modelName, ID3D11Device* g_dev, ID3D11DeviceContext* g_devcon); 
	static SpriteFont* MakeFont(ID3D11Device* g_dev, const wchar_t* fontName);
	static ID3D11VertexShader* CreateVertexShader(ID3D11Device* g_dev,const wchar_t* vertexShaderName, LPCSTR entrypoint, ID3D11InputLayout** il);
	static ID3D11PixelShader* CreatePixelShader(ID3D11Device* g_dev,const wchar_t* pixelShaderName, LPCSTR entrypoint);


private:
	AssetManager() = delete; 
	~AssetManager() = delete;
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;

	///DETERMINES WHETHER THE ASSET HAS ALREADY BEEN LOADED OR NOT BEFORE MAKING A NEW ONE 
	static bool IsTextureLoaded(const wchar_t& textureName);
	static bool IsModelLoaded(const wchar_t& modelName);
	static bool IsFontLoaded(const wchar_t& fontName);
	static bool IsVertexShaderLoaded(const wchar_t& vertexShaderName);
	static bool IsPixelShaderLoaded(const wchar_t& pixelShaderName);


	///RETURNS THE ASSET IF IT HAS ALREADY BEEN LOADED 
	static ID3D11VertexShader* RetrieveVertexShader(const wchar_t& vertexShaderName);
	static ID3D11PixelShader* RetrievePixelShader(const wchar_t& pixelShaderName);
	static std::shared_ptr<Material> RetrieveTexture(const wchar_t& textureName);
	static ObjFileModel* RetrieveModel(const wchar_t& modelName);
	static SpriteFont* RetrieveFont(const wchar_t& fontName);
	
	//had to do some mad shit to make sure these only get initialised on first use otherwise whole window doesnt even open 
	static std::unordered_map<const wchar_t*, std::shared_ptr<Material>>& GetTextures() { static std::unordered_map<const wchar_t*, std::shared_ptr<Material>> m_textures; return m_textures; }
	static std::unordered_map<const wchar_t*,ObjFileModel*>& GetModels() { static std::unordered_map<const wchar_t*, ObjFileModel*> m_models; return m_models; }
	static std::unordered_map<const wchar_t*, SpriteFont*>& GetFonts() { static std::unordered_map<const wchar_t*, SpriteFont*> m_fonts; return m_fonts; }
	static std::unordered_map<const wchar_t*, ID3D11VertexShader*>& GetVertexShaders() { static std::unordered_map<const wchar_t*, ID3D11VertexShader*> m_vertexShaders; return m_vertexShaders; }
	static std::unordered_map<const wchar_t*, ID3D11PixelShader*>& GetPixelShaders() { static std::unordered_map<const wchar_t*, ID3D11PixelShader*> m_pixelShaders; return m_pixelShaders; }

	 
};

