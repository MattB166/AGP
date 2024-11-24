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
	g_devcon->UpdateSubresource(m_CBuffer, 0, 0, &m_CBuffer, 0, 0); // This is doo doo.
	// UpdateSubresource(destination, 0, 0, source, 0, 0)
	// Now your call here doesnt make sense as your dst and src are both the same.
	// You're effectively copying data A into data A...
	// If you look in Renderer::RenderFrame, you will see that you are already calling UpdateSubresource(..)
	// but the difference is you're creating a local cbuffer variable, setting data inside it, THEN
	// copying it into the ID3D11Buffer. So first order of business, is decide if you wanna update and set
	// the cbuffer here in the GameObject, or RenderFrame. I'm kinda leaning to here, but you know your code base better.
	// 
	// Now the other issue, dx error:
	// "The size of the Constant Buffer at slot 0 of the Vertex Shader unit is too small (192 bytes provided, 496 bytes, at least, expected)."
	// This suggests that you are setting the wrong cbuffer in VSSetConstantBuffers(..).
	// Your shader is expecting the cbuffer containing lighting info, very same one you are also using in RenderFrame, so that's good.
	// However, you are changing it to the wrong cbuffer on the line below as m_CBuffer is created here ---------
	// This CBuffer type below is a struct inside of GameObject.h, and does not									|
	// match the one your shader expects (expected 496 bytes but given 192).									|
	// So the other thing is to make sure you are using the same cbuffer in hlsl and cpp						|
	//																											|
	g_devcon->VSSetConstantBuffers(0, 1, &m_CBuffer);							//								|
}																				//								|
																				//								|
void GameObject::CreateConstantBuffer(ID3D11Device* g_dev)						//								|
{																				//								|
	D3D11_BUFFER_DESC cBufferDesc = {};											//								|
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;									//								|
	cBufferDesc.ByteWidth = sizeof(CBuffer); // <----- 192 bytes ------------------------------------------------
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	HRESULT hr = g_dev->CreateBuffer(&cBufferDesc, NULL, &m_CBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error creating constant buffer", L"Error", MB_OK);
	}
}
