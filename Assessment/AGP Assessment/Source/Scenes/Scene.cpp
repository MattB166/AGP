#include "Scene.h"
#include "../AssetManager/AssetManager.h"
#include <iostream>

Scene::Scene()
{
	//initialize the camera.
	m_camera = new Camera();
}

Scene::~Scene()
{
	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr;
	}
}

void Scene::Initialize()
{
	//for each gameobject in the scene, initialize it. 
	for (auto go : m_gameObjects)
	{
		//call initialize.
	}

}

void Scene::ChangeSkyBox(std::shared_ptr<SkyBox> sb)
{
	m_skyBox = sb;
}

void Scene::DrawStatics()
{
	
	//for each gameobject in the scene, draw it and the skybox.
	for (auto go : m_gameObjects)
	{
		//call draw.
	}
	//if the skybox is not null, draw it.
	if (m_skyBox && m_camera)
	{
		//std::cout << "Drawing Skybox" << std::endl;
		m_skyBox->Draw(m_camera);
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
