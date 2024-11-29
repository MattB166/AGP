#include "GameObject.h"
#include <iostream>
#include "../Time/TestTime.h"
#include "../../AssetManager.h"
std::vector<GameObject*> GameObject::Handler::m_gameObjects;

GameObject::GameObject() : m_model(nullptr), m_material(nullptr) //initialise the model pointer to nullptr so that it can be deleted in the destructor.
{
	
}

GameObject::~GameObject()
{

}

GameObject::GameObject(ID3D11Device* dev, ID3D11DeviceContext* devcon, ID3D11Buffer* rendererBuffer, ObjFileModel* model, XMFLOAT3 pos/*const wchar_t* textureMat*/)
{
	//remember to initialise the model pointer and make "new" model before passing here. or alternatively make it new here so can destroy it here too. r
	// will also need parameters for device and device context to create the buffers and texture etc. 
	std::cout << "GameObject created" << std::endl;
	SetPosition(pos.x, pos.y, pos.y);
	SetScale(0.1, 0.1, 0.1);
	CreateConstantBuffer(dev,rendererBuffer);
	m_model = model; // do this through asset manager and let derived classes load in their models and materials. 
					//load in material here
	m_material = AssetManager::CreateTexture(L"ExternalModels/Box.bmp", dev,devcon, L"ReflectiveVertexShader.hlsl", L"ReflectivePixelShader.hlsl");
	if (m_material != nullptr) //would go in start function 
	{
		
		GameObject::Handler::AddGameObject(this);
	}



}
GameObject::GameObject(const wchar_t* TextureName, ID3D11Device& dev, ID3D11DeviceContext& devcon, ID3D11ShaderResourceView* texture)
{
	
}
void GameObject::Start()
{
	if (m_material != nullptr) 
	{
		GameObject::Handler::AddGameObject(this); //prevents attempting to draw a game object that has no material. 
	}
}
void GameObject::Clean()
{
	//need to clean up the buffers and texture here. 
	/*if (m_CBuffer)
	{
		m_CBuffer->Release();
	}*/
	if (m_model)
	{
		delete m_model;
	}
	/*if (m_material)
	{
		delete m_material;
	}*/
}
void GameObject::Draw(ID3D11DeviceContext* g_devcon, ID3D11Buffer* rendererBuffer, const XMMATRIX& view, const XMMATRIX& projection)
{
	//lighting 
	m_cBufferData.ambientLightCol = { 0.5f,0.5f,0.5f,1.0f };
	XMVECTOR directionalLightShinesFrom = { 0.2788f,0.7063f,0.6506f }; //make this a member so can adjust it in runtime. 
	XMMATRIX transpose = XMMatrixTranspose(m_transform.GetWorldMatrix());
	m_cBufferData.directionalLightDir = XMVector3Transform(directionalLightShinesFrom,transpose);

	for (size_t i = 0; i < MAX_POINT_LIGHTS;i++)
	{
		if (!m_cBufferData.pointLights[i].enabled)
			continue;

		XMMATRIX inverse = XMMatrixInverse(nullptr, m_transform.GetWorldMatrix());
		m_cBufferData.pointLights[i].position = XMVector3Transform(m_cBufferData.pointLights[i].position, inverse);
		m_cBufferData.pointLights[i].colour = m_cBufferData.pointLights[i].colour;
		m_cBufferData.pointLights[i].strength = m_cBufferData.pointLights[i].strength;
		m_cBufferData.pointLights[i].enabled = m_cBufferData.pointLights[i].enabled;

	}
	UpdateConstantBuffer(g_devcon,rendererBuffer,view,projection);
	//ApplyGravity();
	if (m_material)
	{
		m_material->Apply(g_devcon);
	}
	else
	{
		return;
	}
	
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

void GameObject::UpdateConstantBuffer(ID3D11DeviceContext* g_devcon, ID3D11Buffer* rendererBuffer, const XMMATRIX& view, const XMMATRIX& projection)
{
	//have a bool to check if the object is reflective or not. so we can assign the correct cbuffer. 
	XMMATRIX world = m_transform.GetWorldMatrix();
	//CBuffer m_cBufferData;
	m_cBufferData.WVP = world * view * projection;
	m_cBufferData.WV = world * view;
	g_devcon->UpdateSubresource(rendererBuffer, 0, 0, &m_cBufferData, 0, 0);
	g_devcon->VSSetConstantBuffers(0, 1, &rendererBuffer);
}

void GameObject::CreateConstantBuffer(ID3D11Device* g_dev, ID3D11Buffer* rendererBuffer) //throws error because renderer cbuffer is different to gameobject cbuffer. 
 
{
	D3D11_BUFFER_DESC cBufferDesc = {};
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.ByteWidth = sizeof(CBuffer);
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	HRESULT hr = g_dev->CreateBuffer(&cBufferDesc, NULL, &rendererBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error creating constant buffer", L"Error", MB_OK);
	}
}
