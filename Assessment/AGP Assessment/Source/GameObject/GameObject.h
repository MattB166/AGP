#pragma once
#include <DirectXMath.h>
//#include "../ObjectFileModel/objfilemodel.h"
#include "../Constant Buffer/CBuffer.h"
#include "../Component/Component.h"
#include <memory>
using namespace DirectX;
struct Transform
{
	XMFLOAT3 pos{ 0,0,0 };
	XMFLOAT3 rot{ 0,0,0 }; 
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
class GameObject
{
public:


private:
	Transform m_transform;
	/*std::unique_ptr<ObjFileModel> m_model = nullptr;*/
	std::unique_ptr<CBuffer> m_constantBuffer = nullptr;
	std::unique_ptr<ReflectiveCBuffer> m_reflectiveConstantBuffer = nullptr;
};

