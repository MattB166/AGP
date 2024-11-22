#pragma once
#include <DirectXMath.h>
#include "../ObjectModel/objfilemodel.h"
#include "../Materials/Material.h"

using namespace DirectX;
	struct Transform ///do i need to put this into a cbuffer?? 
	{
		XMFLOAT3 pos{ 0,0,0 }; 
		XMFLOAT3 rot{ 0,0,0 }; //in radians
		XMFLOAT3 scl{ 1,1,1 };
		XMMATRIX GetWorldMatrix() const
		{
			XMMATRIX translation = XMMatrixTranslation(pos.x, pos.y, pos.z);
			XMMATRIX rotationX = XMMatrixRotationX(rot.x);
			XMMATRIX rotationY = XMMatrixRotationY(rot.y);
			XMMATRIX rotationZ = XMMatrixRotationZ(rot.z);
			XMMATRIX scale = XMMatrixScaling(scl.x, scl.y, scl.z);
			XMMATRIX world = scale * rotationX * rotationY * rotationZ * translation; //world can be a member of cbuffer? 
			return world;
		}
	};
struct CBuffer
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
	/*Transform transform;*/
};
class GameObject ///need a cbuffer ??? 
{
public:
	class Handler //manages all the game objects. might not be used, just testing  
	{
	public: 
		static void AddGameObject(GameObject* gameObject) { m_gameObjects.push_back(gameObject); }
		static void Draw(ID3D11DeviceContext* g_devcon, const XMMATRIX& view, const XMMATRIX& projection)
		{
			for (auto& gameObject : m_gameObjects)
			{
				gameObject->Draw(g_devcon,view,projection);
			}
		}
		static void Clean()
		{
			for (auto& gameObject : m_gameObjects)
			{
				gameObject->Clean();
			}
		}
	private:
		static std::vector<GameObject*> m_gameObjects; //dont forget to define in cpp 
	};

	GameObject();
	~GameObject();
	GameObject(ID3D11Device* dev,ObjFileModel* model, XMFLOAT3 pos /*const wchar_t* textureMat*/); // or use the file path?? needs to be accessible within the texture handler 
	GameObject(const wchar_t* TextureName, ID3D11Device& dev, ID3D11DeviceContext& devcon, ID3D11ShaderResourceView* texture);
	void Clean();
	void Draw(ID3D11DeviceContext* g_devcon, const XMMATRIX& view, const XMMATRIX& projection);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void ApplyGravity();
	void ApplyForce(XMFLOAT3 force);
	ObjFileModel* GetModel() { return m_model; } 
	Material* GetMaterial() { return m_material; }
	Transform GetTransform() { return m_transform; }

private:
	CBuffer m_cBuffer;
	Transform m_transform;
	Material* m_material; // this is the material, so vertex and index buffer not handled here
	ObjFileModel* m_model; // this is the mesh, so vertex and index buffer already handled here 
	ID3D11Buffer* m_CBuffer = nullptr; //this is the constant buffer for the game object. 

	void UpdateConstantBuffer(ID3D11DeviceContext* g_devcon, const XMMATRIX& view, const XMMATRIX& projection);
	void CreateConstantBuffer(ID3D11Device* g_dev);

};

