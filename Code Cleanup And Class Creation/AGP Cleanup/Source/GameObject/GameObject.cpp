#include "GameObject.h"
#include "../Time/TestTime.h"

GameObject::GameObject() : m_model(nullptr)
{
	
}

GameObject::GameObject(ObjFileModel* model, XMFLOAT3 pos)
{
	//remember to initialise the model pointer and make "new" model before passing here. or alternatively make it new here so can destroy it here too. r
	// will also need parameters for device and device context to create the buffers and texture etc. 
	SetPosition(pos.x, pos.y, pos.y);
	SetScale(1, 1, 1);
	m_model = model;
}
GameObject::GameObject(const wchar_t* TextureName, ID3D11Device& dev, ID3D11DeviceContext& devcon, ID3D11ShaderResourceView* texture)
{
	
}
void GameObject::Draw()
{
	GetModel()->Draw();
}
void GameObject::SetPosition(float x, float y, float z)
{
	m_transform.pos = { x,y,z };
}

void GameObject::SetRotation(float x, float y, float z)
{
	m_transform.rot = { x,y,z };
}

void GameObject::SetScale(float x, float y, float z)
{
	m_transform.scl = { x,y,z };
}

void GameObject::ApplyGravity()
{
	m_transform.pos.y += -9.81 * TestTime::getDeltaTime();
}

void GameObject::ApplyForce(XMFLOAT3 force)
{
	XMFLOAT3 NewForce = { force.x, force.y, force.z };
	m_transform.pos.x += NewForce.x * TestTime::getDeltaTime();
	m_transform.pos.y += NewForce.y * TestTime::getDeltaTime();
	m_transform.pos.z += NewForce.z * TestTime::getDeltaTime(); 
}