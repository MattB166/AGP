#include "GameObject.h"

GameObject::GameObject()
{
}

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
	component->SetOwner(this);
	m_components.push_back(component);
}

void GameObject::RemoveComponent(std::shared_ptr<Component> component)
{
	auto it = std::find(m_components.begin(), m_components.end(), component);
	if (it != m_components.end())
	{
		m_components.erase(it);
	}
}

void GameObject::Update(float deltaTime)
{
	for (auto& component : m_components)
	{
		component->Apply();
	}
}
