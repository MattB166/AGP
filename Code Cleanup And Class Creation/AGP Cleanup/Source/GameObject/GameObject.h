#pragma once
#include <DirectXMath.h>
#include "../ObjectModel/objfilemodel.h"

using namespace DirectX;

class GameObject
{
public:
	class Handler
	{
		//draw function which should just call the draw on the model, as that handles that 
		//update function
		//clean up function 
		//etc 
	};
	struct Transform
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
			XMMATRIX world = scale * rotationX * rotationY * rotationZ * translation;
			return world;
		}
	};

	GameObject();
	~GameObject();
	GameObject(ObjFileModel* model, XMFLOAT3 pos);
	GameObject(const wchar_t* TextureName, ID3D11Device& dev, ID3D11DeviceContext& devcon, ID3D11ShaderResourceView* texture);
	void Draw();
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void ApplyGravity();
	void ApplyForce(XMFLOAT3 force);
	ObjFileModel* GetModel() { return m_model; } 
	Transform GetTransform() { return m_transform; }

private:
	Transform m_transform;
	ObjFileModel* m_model; // this is the mesh, so vertex and index buffer already handled here 

};

