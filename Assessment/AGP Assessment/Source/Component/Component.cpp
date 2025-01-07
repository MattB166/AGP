#include "Component.h"
std::vector<ComponentType> Component::m_types = { ComponentType::Model,ComponentType::Texture,ComponentType::Shaders };


std::string Component::ComponentTypeToString(ComponentType type)
{
	switch (type)
	{
	case ComponentType::Model:
		return "Model";
	case ComponentType::Shaders:
		return "Shaders";
	case ComponentType::Texture:
		return "Texture";
	default:
		return "Unknown";
	}
}

