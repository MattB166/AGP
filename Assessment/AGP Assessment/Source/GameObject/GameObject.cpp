#include "GameObject.h"

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
	component->SetOwner(this);
	m_components.push_back(component);
}

void GameObject::Update(float deltaTime)
{
	for (auto& component : m_components)
	{
		component->Apply();
	}
}
