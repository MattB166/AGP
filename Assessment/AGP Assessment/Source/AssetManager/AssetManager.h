#pragma once
#include <d3d11.h>
#include <memory>
#include <unordered_map>
#include "SpriteFont.h"
#include "../Component/Material.h"
#include "../ObjectFileModel/objfilemodel.h"
class AssetManager
{
public:
	static void Initialize(ID3D11Device* dev, ID3D11DeviceContext* devcon);
	static void CleanUp();

#pragma region Externals
	static std::shared_ptr<Material> CreateMaterial(const wchar_t* texturePath);

	static std::shared_ptr<ObjFileModel> CreateModel(const wchar_t* modelPath);

	static std::shared_ptr<SpriteFont> MakeFont(const wchar_t* fontPath);


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
#pragma endregion

#pragma region InternalRetrievals
	static std::shared_ptr<Material> RetrieveMaterial(const wchar_t& texturePath);

	static std::shared_ptr<ObjFileModel> RetrieveModel(const wchar_t& modelPath);

	static std::shared_ptr<SpriteFont> RetrieveFont(const wchar_t& fontPath);
#pragma endregion

#pragma region Maps
	static std::unordered_map<const wchar_t*, std::shared_ptr<Material>> m_materials;

	static std::unordered_map<const wchar_t*, std::shared_ptr<ObjFileModel>> m_models;

	static std::unordered_map<const wchar_t*, std::shared_ptr<SpriteFont>> m_fonts;
#pragma endregion

#pragma region InternalGetters
	static std::unordered_map<const wchar_t*, std::shared_ptr<Material>>& GetMaterials() { return m_materials; }

	static std::unordered_map<const wchar_t*, std::shared_ptr<ObjFileModel>>& GetModels() { return m_models; }

	static std::unordered_map<const wchar_t*, std::shared_ptr<SpriteFont>>& GetFonts() { return m_fonts; }
#pragma endregion
};

