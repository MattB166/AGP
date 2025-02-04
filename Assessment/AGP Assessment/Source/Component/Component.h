#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <d3d11.h>
#include <iostream>
#include <memory>
#include "../IMGUI/imgui.h"
#include <vector>
#include <unordered_map>
#include <experimental/filesystem>
using namespace std::experimental::filesystem;

class GameObject;
enum class ComponentType
{
	Model,
	Shaders,
	Texture,
	None

};
class Component
{
public:
	virtual ~Component() = default;
	virtual void Apply() = 0;
	virtual void ShowDebugWindow() = 0;
	virtual std::string GetComponentFilePath(const std::string& name)const = 0;
	void SetOwner(GameObject* owner) { m_owner = owner; }
	GameObject* GetOwner() { return m_owner; }
	ComponentType GetType() { return m_type; }
	static std::vector<ComponentType> GetTypes() { return m_types; }
	static std::string ComponentTypeToString(ComponentType type); 
	static ComponentType StringToType(const std::string& type)
	{
		if (type == "Model")
			return ComponentType::Model;
		else if (type == "Shaders")
			return ComponentType::Shaders;
		else if (type == "Texture")
			return ComponentType::Texture;
		else
			return ComponentType::Model;
	}
	virtual std::vector<std::string> GetComponentOptions() const = 0; 
protected:
	Component(ID3D11Device* dev, ID3D11DeviceContext* devcon, const char* name, ComponentType type) : m_dev(dev), m_devcon(devcon),m_name(name),m_type(type) {}
	std::string m_name = nullptr;
	GameObject* m_owner = nullptr;
	ID3D11Device* m_dev = nullptr;
	ID3D11DeviceContext* m_devcon = nullptr; 
	ComponentType m_type;
	static std::vector<ComponentType> m_types;
};

