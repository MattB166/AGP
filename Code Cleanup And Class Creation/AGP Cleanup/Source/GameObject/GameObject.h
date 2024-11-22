#pragma once
#include <DirectXMath.h>
#include "../ObjectModel/objfilemodel.h"
#include "../Materials/Material.h"

using namespace DirectX;

class GameObject ///need a cbuffer ??? 
{
public:
	class Handler //manages all the game objects. might not be used, just testing  
	{
	public: 
		static void AddGameObject(GameObject* gameObject) { m_gameObjects.push_back(gameObject); }
		static void Draw(ID3D11DeviceContext* g_devcon)
		{
			for (auto& gameObject : m_gameObjects)
			{
				gameObject->Draw(g_devcon);
			}
		}
	private:
		static std::vector<GameObject*> m_gameObjects; //dont forget to define in cpp 
	};
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

	GameObject();
	~GameObject();
	GameObject(ObjFileModel* model, XMFLOAT3 pos /*const wchar_t* textureMat*/); // or use the file path?? needs to be accessible within the texture handler 
	GameObject(const wchar_t* TextureName, ID3D11Device& dev, ID3D11DeviceContext& devcon, ID3D11ShaderResourceView* texture);
	void Draw(ID3D11DeviceContext* g_devcon);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void ApplyGravity();
	void ApplyForce(XMFLOAT3 force);
	ObjFileModel* GetModel() { return m_model; } 
	Material* GetMaterial() { return m_material; }
	Transform GetTransform() { return m_transform; }

private:
	Transform m_transform;
	Material* m_material; // this is the material, so vertex and index buffer not handled here
	ObjFileModel* m_model; // this is the mesh, so vertex and index buffer already handled here 
	ID3D11Buffer* m_CBuffer = NULL; //this is the constant buffer for the game object. 

};

