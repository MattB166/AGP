#pragma once
#include <DirectXMath.h>
//#include "../ObjectFileModel/objfilemodel.h"
#include "../Constant Buffer/CBuffer.h"
#include "../Component/Component.h"
#include <memory>
#include <vector>
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
	GameObject(const char* name); //need to find way to give it default model	
	~GameObject();
	void Initialise(); //initialise the gameobject and its position, rotation and scale.
	void AddComponent(std::shared_ptr<Component> component);
	void RemoveComponent(std::shared_ptr<Component> component);
	void Draw(XMMATRIX& view, XMMATRIX& proj);
	void ShowComponentDebugWindow();
	//void Update(float deltaTime);
	void SetPosition(float x, float y, float z) { m_transform.pos = { x,y,z }; }
	void SetRotation(float x, float y, float z) { m_transform.rot = { x,y,z }; }
	void SetScale(float x, float y, float z) { m_transform.scl = { x,y,z }; }
	void ChangeYPosition(float y) { m_transform.pos.y += y; }
	void ChangeXPosition(float x) { m_transform.pos.x += x; }
	void ChangeZPosition(float z) { m_transform.pos.z += z; }
	void ChangeAmbientLight(float value) { BasicAmbientLightValue += value; }


	Transform GetTransform() { return m_transform; }
	const char* GetName() { return m_name; }


private:
	const char* m_name; 
	Transform m_transform;
	CBuffer m_constantBuffer;  //only one type of cbuffer required for gameobject. shader will know what to do with it. 
	std::vector<std::shared_ptr<Component>> m_components;
	float BasicAmbientLightValue = 0.5f;
	float DebugMovementSnappingValue;
};

