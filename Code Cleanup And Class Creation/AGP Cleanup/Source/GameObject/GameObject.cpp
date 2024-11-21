#include "GameObject.h"

GameObject::GameObject() : m_model(nullptr), m_texture(nullptr)
{

}

GameObject::GameObject(ObjFileModel* model)
{
	//remember to initialise the model pointer and make "new" model before passing here. or alternatively make it new here so can destroy it here too. r
	// will also need parameters for device and device context to create the buffers and texture etc. 
	m_model = model;
}
GameObject::GameObject(const wchar_t* TextureName, ID3D11Device& dev, ID3D11DeviceContext& devcon, ID3D11ShaderResourceView* texture)
{
	m_texture = texture;
	//create the buffers here 
	//create the model here 
	//create the shader here 
	//create the material here 
	//create the texture here 
	//etc 
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

void GameObject::InitBuffers(ID3D11Device& dev, ID3D11DeviceContext& devcon)
{
}

void GameObject::InitShaders(ID3D11Device& dev, ID3D11DeviceContext& devcon)
{
}

void GameObject::InitTexture(ID3D11Device& dev, ID3D11DeviceContext& devcon)
{
}
