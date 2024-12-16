#pragma once
#include <d3d11.h>
#include <memory>
class GameObject;
class Component
{
public:
	virtual ~Component() = default;
	virtual void Apply() = 0;

	void SetOwner(GameObject* owner) { m_owner = owner; }
	GameObject* GetOwner() { return m_owner; }
protected:
	Component(ID3D11Device* dev, ID3D11DeviceContext* devcon) : m_dev(dev), m_devcon(devcon) {}
	GameObject* m_owner = nullptr;
	ID3D11Device* m_dev = nullptr;
	ID3D11DeviceContext* m_devcon = nullptr; 
};

