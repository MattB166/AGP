#pragma once
#include "../GameObject/GameObject.h"
class Scene //controls all current gameobjects in the scene. 
{
public:
	void Initialize(); //initialize the scene.
	void DrawStatics(); //draw all gameobjects in the scene. 
	void CycleSelectedGameObject(); //cycle through the gameobjects in the scene. 

private:
	//vector of gameobjects, and selected gameobject.
	std::vector<GameObject*> m_gameObjects;
	GameObject* m_selectedGameObject = nullptr;
	int m_selectedIndex = 0;
};

