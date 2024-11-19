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
	GameObject(ObjFileModel* model);
	GameObject(const wchar_t* TextureName, ID3D11Device& dev, ID3D11DeviceContext& devcon, ID3D11ShaderResourceView* texture);
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	ObjFileModel* GetModel() { return m_model; } 
	Transform GetTransform() { return m_transform; }
	ID3D11ShaderResourceView* GetTexture() { return m_texture; }

private:
	Transform m_transform;
	ObjFileModel* m_model;
	ID3D11ShaderResourceView* m_texture;
	//vertex buffer 
	//index buffer 
	//texture 
	//shader 
	//material 
	//model 
	//etc
};

