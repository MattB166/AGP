#pragma once
#include "../GameObject/GameObject.h"
class Scene //controls all current gameobjects in the scene. 
{
public:
	Scene() = default;
	virtual ~Scene() = default;
	virtual void OnLoad() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnRender() = 0;
	virtual void OnImGuiRender() = 0;
	virtual void OnUnload() = 0;   

private:
	//vector of gameobjects. 
};

