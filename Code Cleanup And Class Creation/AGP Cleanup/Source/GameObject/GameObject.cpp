#include "GameObject.h"
#include "../Time/TestTime.h"

GameObject::GameObject() : m_model(nullptr), m_cBuffer(), m_material(nullptr), m_CBuffer(nullptr)
{
	
}

GameObject::~GameObject()
{

}

GameObject::GameObject(ID3D11Device* dev, ObjFileModel* model, XMFLOAT3 pos /*const wchar_t* textureMat*/)
{
	//remember to initialise the model pointer and make "new" model before passing here. or alternatively make it new here so can destroy it here too. r
	// will also need parameters for device and device context to create the buffers and texture etc. 
	SetPosition(pos.x, pos.y, pos.y);
	SetScale(0.2, 0.2, 0.2);
	CreateConstantBuffer(dev);
	m_model = model;
	//handle material loading here. so search from asset handler for the material. the handler will search for it, and if it doesnt exist, it will create it. 
}
GameObject::GameObject(const wchar_t* TextureName, ID3D11Device& dev, ID3D11DeviceContext& devcon, ID3D11ShaderResourceView* texture)
{
	
}
void GameObject::Clean()
{
	//need to clean up the buffers and texture here. 
	if (m_CBuffer)
	{
		m_CBuffer->Release();
	}
	if (m_model)
	{
		delete m_model;
	}
	if (m_material)
	{
		delete m_material;
	}
}
void GameObject::Draw(ID3D11DeviceContext* g_devcon, const XMMATRIX& view, const XMMATRIX& projection)
{
	UpdateConstantBuffer(g_devcon,view,projection);
	g_devcon->VSSetConstantBuffers(0, 1, &m_CBuffer);
	GetModel()->Draw();
	//use g_dev to update material and buffers etc.
	// //need a cbuffer 
	//also needs to set the constant buffers and texture here. pass gdev / devcon to the model draw function, so it can set the buffers and texture??
	//update subresource for the constant buffer here too. 
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

void GameObject::UpdateConstantBuffer(ID3D11DeviceContext* g_devcon, const XMMATRIX& view, const XMMATRIX& projection)
{
	m_cBuffer.world = XMMatrixTranspose(m_transform.GetWorldMatrix());
	m_cBuffer.view = XMMatrixTranspose(view);
	m_cBuffer.projection = XMMatrixTranspose(projection);
	g_devcon->UpdateSubresource(m_CBuffer, 0, 0, &m_CBuffer, 0, 0);
	g_devcon->VSSetConstantBuffers(0, 1, &m_CBuffer);
}

void GameObject::CreateConstantBuffer(ID3D11Device* g_dev)
{
	D3D11_BUFFER_DESC cBufferDesc = {};
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.ByteWidth = sizeof(CBuffer);
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	HRESULT hr = g_dev->CreateBuffer(&cBufferDesc, NULL, &m_CBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error creating constant buffer", L"Error", MB_OK);
	}
}
