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

void Scene::ChangeActiveSkyBox(std::shared_ptr<SkyBox> sb)
{
	m_ActiveSkyBox = sb;
}

void Scene::AddSkyBoxToScene(std::shared_ptr<SkyBox> sb)
{
	m_skyBoxes.push_back(sb);
	//std::cout << "Skybox Added" << std::endl;
	//std::cout << "Skybox Count: " << m_skyBoxes.size() << std::endl;
}

void Scene::RemoveSkyBoxFromScene(std::shared_ptr<SkyBox> sb)
{
	//if there are no skyboxes, return.
	if (m_skyBoxes.empty() || m_skyBoxes.size() < 2)
	{
		return;
	}
	//find the skybox in the vector.
	auto it = std::find(m_skyBoxes.begin(), m_skyBoxes.end(), sb);
	//if the skybox is found, remove it.
	if (it != m_skyBoxes.end())
	{
		if (m_ActiveSkyBox == sb)
		{
			CycleThroughSkyBoxes();
		}
		m_skyBoxes.erase(it);
		//std::cout << "Skybox Removed" << std::endl;
	}
}

void Scene::CycleThroughSkyBoxes()
{
	//if there are no skyboxes, return.
	if (m_skyBoxes.empty() || m_skyBoxes.size() == 1)
	{
		return;
	}
	m_skyBoxIndex = (m_skyBoxIndex + 1) % m_skyBoxes.size();
	ChangeActiveSkyBox(m_skyBoxes[m_skyBoxIndex]);
	//std::cout << "Skybox Changed" << std::endl;

}

void Scene::DrawStatics()
{
	
	//for each gameobject in the scene, draw it and the skybox.
	for (auto go : m_gameObjects)
	{
		//call draw.
	}
	//if the skybox is not null, draw it.
	if (m_ActiveSkyBox && m_camera)
	{
		//std::cout << "Drawing Skybox" << std::endl;
		m_ActiveSkyBox->Draw(m_camera);
	}
}

void Scene::CycleSelectedGameObject()
{
	//if there are no gameobjects, return.
	if (m_gameObjects.empty())
	{
		return;
	}
	const int CurrentIndex = m_selectedObjectIndex;
	const int NextIndex = (m_selectedObjectIndex + 1) % m_gameObjects.size();
	m_selectedGameObject = m_gameObjects[NextIndex]; 
}
