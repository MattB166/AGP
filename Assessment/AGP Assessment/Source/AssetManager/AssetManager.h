#pragma once
#include <d3d11.h>
#include <memory>
#include <unordered_map>
#include "../Component/Material.h"
class AssetManager
{
public:
	static void Initialize(ID3D11Device* dev, ID3D11DeviceContext* devcon);

#pragma region Externals
	static std::shared_ptr<Material> CreateMaterial(ID3D11Device* dev, ID3D11DeviceContext* devcon, const wchar_t* texturePath);


#pragma endregion
private:
	static ID3D11Device* m_dev;
	static ID3D11DeviceContext* m_devcon;

	AssetManager() = delete;
	~AssetManager() = delete;
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;

#pragma region InternalChecks
	static bool IsMaterialLoaded(const wchar_t& texturePath);
#pragma endregion

#pragma region InternalRetrievals
	static std::shared_ptr<Material> RetrieveMaterial(const wchar_t& texturePath);
#pragma endregion

#pragma region Maps
	static std::unordered_map<const wchar_t*, std::shared_ptr<Material>> m_materials;
#pragma endregion

#pragma region InternalGetters
	static std::unordered_map<const wchar_t*, std::shared_ptr<Material>>& GetMaterials() { return m_materials; }
#pragma endregion
};

