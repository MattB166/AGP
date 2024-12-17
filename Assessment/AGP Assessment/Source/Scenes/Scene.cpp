#include "Scene.h"

void Scene::Initialize()
{
	//for each gameobject in the scene, initialize it. 
	for (auto go : m_gameObjects)
	{
		//call initialize.
	}

}

void Scene::DrawStatics()
{
	//for each gameobject in the scene, draw it without its behaviour. 
	for (auto go : m_gameObjects)
	{
		//call draw.
	}
}

void Scene::CycleSelectedGameObject()
{
	//if there are no gameobjects, return.
	if (m_gameObjects.empty())
	{
		return;
	}
	const int CurrentIndex = m_selectedIndex;
	const int NextIndex = (m_selectedIndex + 1) % m_gameObjects.size();
	m_selectedGameObject = m_gameObjects[NextIndex]; 
}
