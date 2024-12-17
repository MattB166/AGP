#pragma once
#include <d3d11.h>
#include <memory>
#include <unordered_map>
#include "SpriteFont.h"
#include "../Component/Material.h"
#include "../Component/Model.h"
#include "../Component/ShaderSet.h"
#include "../ReadData/ReadData.h"

class AssetManager
{
public:
	static void Initialize(ID3D11Device* dev, ID3D11DeviceContext* devcon);
	static void CleanUp();

#pragma region Externals
	static std::shared_ptr<Material> CreateMaterial(const wchar_t* texturePath);

	static std::shared_ptr<Model> CreateModel(const wchar_t* modelPath);

	static std::shared_ptr<SpriteFont> MakeFont(const wchar_t* fontPath);

	static std::shared_ptr<ShaderSet> CreateShaderSet(const wchar_t* vsPath, const wchar_t* psPath);


#pragma endregion

private:
#pragma region Members
	static ID3D11Device* m_dev;
	static ID3D11DeviceContext* m_devcon;
#pragma endregion

#pragma region ConstructorsAndOperators
	AssetManager() = delete;
	~AssetManager() = delete;
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;
#pragma endregion	

#pragma region InternalChecks
	static bool IsMaterialLoaded(const wchar_t& texturePath);

	static bool IsModelLoaded(const wchar_t& modelPath); 

	static bool IsFontLoaded(const wchar_t& fontPath);

	static bool IsShaderSetLoaded(const wchar_t& vsPath, const wchar_t& psPath);
#pragma endregion

#pragma region InternalRetrievals
	static std::shared_ptr<Material> RetrieveMaterial(const wchar_t& texturePath);

	static std::shared_ptr<Model> RetrieveModel(const wchar_t& modelPath);

	static std::shared_ptr<SpriteFont> RetrieveFont(const wchar_t& fontPath);

	static std::shared_ptr<ShaderSet> RetrieveShaderSet(const wchar_t& vsPath, const wchar_t& psPath);
#pragma endregion

#pragma region Maps
	static std::unordered_map<const wchar_t*, std::shared_ptr<Material>> m_materials;

	static std::unordered_map<const wchar_t*, std::shared_ptr<Model>> m_models;

	static std::unordered_map<const wchar_t*, std::shared_ptr<SpriteFont>> m_fonts;

	static std::unordered_map<std::string, std::shared_ptr<ShaderSet>> m_shaderSets;
#pragma endregion

#pragma region InternalGetters
	static std::unordered_map<const wchar_t*, std::shared_ptr<Material>>& GetMaterials() { return m_materials; }

	static std::unordered_map<const wchar_t*, std::shared_ptr<Model>>& GetModels() { return m_models; }

	static std::unordered_map<const wchar_t*, std::shared_ptr<SpriteFont>>& GetFonts() { return m_fonts; }

	static std::unordered_map<std::string, std::shared_ptr<ShaderSet>>& GetShaderSets() { return m_shaderSets; }
#pragma endregion

#pragma region InternalFunctions

	static std::string GenerateKeyForShaderSet(const wchar_t* vsPath, const wchar_t* psPath);
};

