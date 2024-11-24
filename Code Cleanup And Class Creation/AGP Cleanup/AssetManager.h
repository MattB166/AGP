#pragma once
#include "../AGP Cleanup/Source/Materials/Material.h"
#include <vector>
class AssetManager //handles storing and loading of assets (textures, models, sounds etc) 
{
public:
	static Material* GetMaterial(const Material& name); //get a material by name from the materials vector. 
	static void AddMaterial(Material* material); //add a material to the materials vector. 
	static void RemoveMaterial(const Material& name); //remove a material by name from the materials vector.
	static void Clean(); //clean up all materials in the materials vector.


private:
	AssetManager() = delete; 
	~AssetManager() = delete;
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;


	static std::vector<Material*> m_materials; //store all materials in the game here. 
};

